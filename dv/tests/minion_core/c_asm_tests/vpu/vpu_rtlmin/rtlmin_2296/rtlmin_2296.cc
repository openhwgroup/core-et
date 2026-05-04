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
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800000,                                                  // inf                                        
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f800000,                                                  // inf                                        
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000000,                                                  // -zero                                      
			 0x80000000,                                                  // -zero                                      
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f800000,                                                  // inf                                        
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c780000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000000,                                                  // zero                                       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000000,                                                  // -zero                                      
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000,                                                  // -inf                                       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800000,                                                  // inf                                        
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x33333333,                                                  // 4 random values                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x33333333,                                                  // 4 random values                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x55555555,                                                  // 4 random values                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f800000,                                                  // inf                                        
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f800000,                                                  // inf                                        
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x33333333,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f800000,                                                  // inf                                        
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000000,                                                  // -zero                                      
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x33333333,                                                  // 4 random values                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x55555555,                                                  // 4 random values                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f800000,                                                  // inf                                        
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x0c700000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800000,                                                  // inf                                        
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001,                                                  // -signaling NaN                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xbf028f5c,                                                  // -0.51                                      
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x33333333,                                                  // 4 random values                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000000,                                                  // -zero                                      
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x33333333,                                                  // 4 random values                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000000,                                                  // -zero                                      
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000000,                                                  // -zero                                      
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000,                                                  // -inf                                       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff800000,                                                  // -inf                                       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff800000,                                                  // -inf                                       
			 0x7f800000,                                                  // inf                                        
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
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
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             

			 // vpu register f3
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f4
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               

			 // vpu register f5
			 0xff800000,                                                  // -inf                                       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f6
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               

			 // vpu register f7
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f8
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             

			 // vpu register f9
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                

			 // vpu register f10
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         

			 // vpu register f11
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f12
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                

			 // vpu register f13
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            

			 // vpu register f14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            

			 // vpu register f15
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                

			 // vpu register f16
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f17
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               

			 // vpu register f18
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f19
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f20
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f21
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f22
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f23
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f24
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f25
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f26
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f27
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               

			 // vpu register f29
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             

			 // vpu register f31
			 0x7f800000,                                                  // inf                                        
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
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
		"fcvt.ps.f16 f17, f0\n"                               // opcode=0xd0a008fb


		"fsq2 f12, -1280(x14)\n"                              // opcode=0xa2c75fa7
		"fmax.s f24, f12, f14\n"                              // opcode=0x28e61c53
		"fsgnjn.s f20, f12, f15\n"                            // opcode=0x20f61a53
		"cubesgntc.ps f21, f13, f12\n"                        // opcode=0x88c6bafb
		"mova.m.x x14\n"                                      // opcode=0xd607107b
		"fclass.ps f16, f15\n"                                // opcode=0xe007987b
		"fsat8.pi f23, f15\n"                                 // opcode=0x0607bbfb
		"fmul.ps f26, f12, f12, dyn\n"                        // opcode=0x10c67d7b
		"feq.ps f17, f12, f1\n"                               // opcode=0xa01628fb
		"fmax.pi f30, f12, f0\n"                              // opcode=0x2e061f7b
		"fsgnjn.ps f26, f1, f0\n"                             // opcode=0x20009d7b
		"flw.ps f1,  32 (x4)\n"
		"fmin.pi f26, f1, f12\n"                              // opcode=0x2ec08d7b
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f25, f15, f13\n"                              // opcode=0xa0d7acfb
		"fbci.ps f20, 125202\n"                               // opcode=0x3d225a1f
		"fbci.pi f25, 180616\n"                               // opcode=0x58310cdf
		"fltm.pi m5, f13, f15\n"                              // opcode=0x3ef682fb
		"maskor m7, m4, m1\n"                                 // opcode=0x661263fb
		"fsgnj.ps f18, f12, f13\n"                            // opcode=0x20d6097b
		"fsub.pi f18, f1, f14\n"                              // opcode=0x0ee0897b
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f23, f0\n"                                 // opcode=0x06103bfb


		"flw f1, 648(x12)\n"                                  // opcode=0x2885a087
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f17, f15, f13\n"                            // opcode=0x20d788fb
		"fmvz.x.ps x27, f14, 7\n"                             // opcode=0xe0770dfb
		"fsll.pi f28, f15, f12\n"                             // opcode=0x06c79e7b




		"fmin.s f26, f14, f14\n"                              // opcode=0x28e70d53
		"fmadd.s f16, f15, f14, f0, dyn\n"                    // opcode=0x00e7f843
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m0, f0\n"                                   // opcode=0xe000507b
		"fcvt.s.wu f18, x15, dyn\n"                           // opcode=0xd017f953
		"fltm.ps m0, f12, f12\n"                              // opcode=0xa0c6507b
		"fsrai.pi f22, f0, 11\n"                              // opcode=0x4f607b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flem.ps m0, f15, f0\n"                               // opcode=0xa007c07b
		"fcvt.un2.ps f17, f14\n"                              // opcode=0xd97708fb
		"fmul.ps f26, f12, f1, rmm\n"                         // opcode=0x10164d7b
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m1, f14, f15\n"                              // opcode=0xa0f750fb
		"mova.m.x x1\n"                                       // opcode=0xd600907b
		"fmadd.ps f26, f12, f0, f1, dyn\n"                    // opcode=0x08067d5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f31, f0, 13\n"                              // opcode=0x4fa05ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x1, m6\n"                                   // opcode=0x520300fb
		"masknot m2, m7\n"                                    // opcode=0x6603a17b


		"flw.ps f18, 1959(x12)\n"                             // opcode=0x7a76a90b
		"fle.s x0, f14, f13\n"                                // opcode=0xa0d70053
		"fsrai.pi f28, f12, 4\n"                              // opcode=0x4e967e7b
		"fsgnj.ps f22, f0, f0\n"                              // opcode=0x20000b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x17\n"                                      // opcode=0xd60008fb
		"maskand m0, m1, m0\n"                                // opcode=0x6600f07b
		"fcvt.un16.ps f26, f1\n"                              // opcode=0xd9108d7b
		"fsrl.pi f25, f13, f14\n"                             // opcode=0x06e6dcfb
		"fcmovm.ps f0, f12, f15\n"                            // opcode=0x00f60077
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw.ps f21, 1727(x13)\n"                             // opcode=0x6bf72a8b
		"fle.pi f30, f14, f14\n"                              // opcode=0xa6e70f7b
		"fand.pi f22, f13, f12\n"                             // opcode=0x06c6fb7b
		"fsatu8.pi f27, f14\n"                                // opcode=0x06173dfb
		"fmv.x.s x24, f12\n"                                  // opcode=0xe0060c53
		"cubefaceidx.ps f24, f14, f15\n"                      // opcode=0x88f71c7b
		"flog.ps f25, f0\n"                                   // opcode=0x58300cfb
		"maskpopcz x18, m3\n"                                 // opcode=0x5401897b
		"fsrai.pi f25, f0, 7\n"                               // opcode=0x4ee07cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f17, f12\n"                                // opcode=0xe00618fb
		"flt.pi f30, f14, f13\n"                              // opcode=0xa6d71f7b
		"fnot.pi f24, f13\n"                                  // opcode=0x0606ac7b
		"ffrc.ps f0, f12\n"                                   // opcode=0x5826007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f1, f0, f15, dyn\n"                           // opcode=0x08f070d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fminu.pi f27, f12, f13\n"                            // opcode=0x2ed62dfb
		"fxor.pi f16, f12, f1\n"                              // opcode=0x0616487b
		"fmv.x.s x24, f0\n"                                   // opcode=0xe0000c53
		"fsub.pi f21, f15, f14\n"                             // opcode=0x0ee78afb
		"fcvt.un10.ps f19, f1\n"                              // opcode=0xd92089fb
		"fmin.ps f21, f14, f15\n"                             // opcode=0x28f70afb
		"fexp.ps f28, f14\n"                                  // opcode=0x58470e7b
		"fminu.pi f26, f12, f13\n"                            // opcode=0x2ed62d7b
		"cubeface.ps f23, f0, f12\n"                          // opcode=0x88c00bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackrepb.pi f24, f0\n"                              // opcode=0x26000c7b
		"fadd.pi f1, f13, f0\n"                               // opcode=0x060680fb
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f31, f1, f0\n"                               // opcode=0x16008ffb
		"flw.ps f1,  32 (x4)\n"
		"feq.s x29, f1, f14\n"                                // opcode=0xa0e0aed3
		"flw.ps f1,  32 (x4)\n"
		"maskxor m2, m7, m7\n"                                // opcode=0x6673c17b
		"fmulhu.pi f25, f15, f0\n"                            // opcode=0x16079cfb
		"fsat8.pi f17, f0\n"                                  // opcode=0x060038fb
		"fltm.pi m7, f0, f13\n"                               // opcode=0x3ed003fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x22, m3\n"                                 // opcode=0x54018b7b
		"fcvt.wu.s x16, f15, dyn\n"                           // opcode=0xc017f853
		//RTLMIN-5479"frsq.ps f21, f13\n"                                  // opcode=0x58868afb
		"fmin.pi f31, f12, f1\n"                              // opcode=0x2e160ffb
		"fsgnjx.s f1, f0, f1\n"                               // opcode=0x201020d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fbci.pi f24, 493583\n"                               // opcode=0xf101fc5f
		"maskor m4, m2, m7\n"                                 // opcode=0x6671627b


		"flw f21, -1629(x14)\n"                               // opcode=0x9a30aa87
		"fsub.ps f19, f13, f12, dyn\n"                        // opcode=0x08c6f9fb
		"fsub.pi f28, f0, f0\n"                               // opcode=0x0e000e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.ps f27, f14, f15\n"                            // opcode=0x20f70dfb
		"fbcx.ps f29, x12\n"                                  // opcode=0x00063e8b
		"fsra.pi f30, f0, f15\n"                              // opcode=0x0ef05f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f26, f12\n"                              // opcode=0xd8960d7b
		"fmul.ps f18, f12, f12, dyn\n"                        // opcode=0x10c6797b
		"fmadd.ps f25, f12, f15, f12, dyn\n"                  // opcode=0x60f67cdb




		"fcvt.wu.s x18, f12, dyn\n"                           // opcode=0xc0167953
		"cubefaceidx.ps f19, f1, f15\n"                       // opcode=0x88f099fb
		"flw.ps f1,  32 (x4)\n"
		"frcp.ps f19, f13\n"                                  // opcode=0x587689fb




		"fcvt.sn8.ps f16, f15\n"                              // opcode=0xd9b7887b
		"fmsub.s f18, f13, f15, f14, rup\n"                   // opcode=0x70f6b947
		"fsll.pi f25, f0, f12\n"                              // opcode=0x06c01cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f18, f15, f15\n"                            // opcode=0x16f7a97b
		"mova.x.m x28\n"                                      // opcode=0xd6000e7b
		"fsetm.pi m6, f1\n"                                   // opcode=0xe000d37b
		"fcmovm.ps f0, f0, f15\n"                             // opcode=0x00f00077
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.s f31, f15, f14\n"                            // opcode=0x20e7afd3
		"fsra.pi f22, f0, f15\n"                              // opcode=0x0ef05b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.s f25, f0, f13\n"                               // opcode=0x28d00cd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pw.ps f27, f1, dyn\n"                           // opcode=0xc000fdfb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.s f1, f0, f0\n"                               // opcode=0x200010d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fnmsub.ps f19, f15, f12, f12, dyn\n"                 // opcode=0x64c7f9db
		"fsll.pi f23, f12, f14\n"                             // opcode=0x06e61bfb
		"fltm.pi m7, f13, f12\n"                              // opcode=0x3ec683fb
		"flt.ps f20, f12, f13\n"                              // opcode=0xa0d61a7b
		"fle.pi f20, f13, f13\n"                              // opcode=0xa6d68a7b




		"fltm.ps m1, f1, f12\n"                               // opcode=0xa0c0d0fb
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f19, f1, 0\n"                               // opcode=0x4e0099fb
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f30, f14, f13\n"                             // opcode=0x16d70f7b
		"fle.pi f31, f13, f13\n"                              // opcode=0xa6d68ffb
		"fmvs.x.ps x26, f12, 6\n"                             // opcode=0xe0662d7b
		"fnmadd.s f24, f14, f0, f1, dyn\n"                    // opcode=0x08077c4f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f0, -2015(x11)\n"                             // opcode=0x8205e08b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgntc.ps f21, f12, f1\n"                         // opcode=0x88163afb
		"fexp.ps f18, f15\n"                                  // opcode=0x5847897b
		"fle.pi f26, f13, f12\n"                              // opcode=0xa6c68d7b
		"fsgnjn.s f25, f13, f14\n"                            // opcode=0x20e69cd3


		"fbc.ps f16, 160(x11)\n"                              // opcode=0x43f5880b


		"fsw f12, 811(x12)\n"                                 // opcode=0x32c725a7
		"fmax.ps f25, f0, f15\n"                              // opcode=0x28f01cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fround.ps f0, f1, dyn\n"                             // opcode=0x5810f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f23, f15, f15\n"                             // opcode=0x06f7cbfb


		"flw f22, -728(x14)\n"                                // opcode=0xd2872b07
		"fsra.pi f17, f1, f15\n"                              // opcode=0x0ef0d8fb
		"flw.ps f1,  32 (x4)\n"
		"fmax.pi f28, f14, f0\n"                              // opcode=0x2e071e7b
		"cubeface.ps f1, f14, f1\n"                           // opcode=0x881700fb
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x18, f14\n"                                 // opcode=0xe0071953




		"fnmsub.s f19, f12, f12, f15, dyn\n"                  // opcode=0x78c679cb






		"fswizz.ps f1, f13, 116\n"                            // opcode=0xe7d690fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.sn8.ps f25, f13\n"                              // opcode=0xd9b68cfb
		"fltm.pi m0, f12, f1\n"                               // opcode=0x3e16007b
		"fcvt.ps.pwu f24, f13, dyn\n"                         // opcode=0xd016fc7b
		"fsgnj.ps f30, f0, f0\n"                              // opcode=0x20000f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f30, f13\n"                              // opcode=0xd8968f7b
		"fand.pi f23, f14, f14\n"                             // opcode=0x06e77bfb
		"fcvt.s.w f31, x0, rdn\n"                             // opcode=0xd0002fd3
		"frcp.ps f24, f0\n"                                   // opcode=0x58700c7b
		"fcvt.s.wu f18, x1, rup\n"                            // opcode=0xd010b953
		"fcvt.f16.ps f0, f13\n"                               // opcode=0xd896807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.sn8.ps f27, f15\n"                              // opcode=0xd9b78dfb
		"fmin.s f27, f15, f13\n"                              // opcode=0x28d78dd3
		"flt.s x23, f12, f12\n"                               // opcode=0xa0c61bd3
		"fmaxu.pi f16, f12, f12\n"                            // opcode=0x2ec6387b
		"feq.ps f26, f12, f15\n"                              // opcode=0xa0f62d7b
		"fmv.s.x f26, x12\n"                                  // opcode=0xf0060d53
		"fle.pi f25, f13, f13\n"                              // opcode=0xa6d68cfb
		"fle.ps f18, f12, f15\n"                              // opcode=0xa0f6097b
		"fmulhu.pi f17, f0, f0\n"                             // opcode=0x160018fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f20, f14, f14\n"                      // opcode=0x88e71a7b
		"fnmsub.s f25, f15, f13, f0, dyn\n"                   // opcode=0x00d7fccb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"for.pi f25, f12, f12\n"                              // opcode=0x06c66cfb
		"fround.ps f24, f0, rdn\n"                            // opcode=0x58102c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f22, f12, f12\n"                            // opcode=0x16c62b7b
		"fclass.s x27, f1\n"                                  // opcode=0xe0009dd3
		"fmax.s f22, f14, f13\n"                              // opcode=0x28d71b53
		"fandi.pi f31, f13, 224\n"                            // opcode=0x74069fbf
		"fsub.pi f1, f14, f13\n"                              // opcode=0x0ed700fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pwu.ps f0, f12, dyn\n"                          // opcode=0xc016707b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m2, m3, m2\n"                                // opcode=0x6621c17b
		"fnot.pi f18, f15\n"                                  // opcode=0x0607a97b
		"fsub.ps f30, f1, f0, dyn\n"                          // opcode=0x0800ff7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubesgntc.ps f1, f14, f1\n"                          // opcode=0x881730fb
		"flw.ps f1,  32 (x4)\n"
		"fmax.ps f23, f12, f14\n"                             // opcode=0x28e61bfb
		"fmadd.ps f30, f13, f14, f13, dyn\n"                  // opcode=0x68e6ff5b
		"fcvt.wu.s x0, f13, dyn\n"                            // opcode=0xc016f053
		"ffrc.ps f22, f14\n"                                  // opcode=0x58270b7b


		"fcvt.un10.ps f26, f13\n"                             // opcode=0xd9268d7b
		"fsgnj.s f20, f15, f1\n"                              // opcode=0x20178a53
		"maskand m5, m6, m0\n"                                // opcode=0x660372fb
		"fcvt.un10.ps f16, f12\n"                             // opcode=0xd926087b
		"fsrl.pi f17, f12, f13\n"                             // opcode=0x06d658fb
		"fmax.s f31, f1, f1\n"                                // opcode=0x28109fd3
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f26, f14, dyn\n"                          // opcode=0xc0077d7b
		"fcvt.wu.s x17, f15, dyn\n"                           // opcode=0xc017f8d3
		//RTLMIN-5479"frsq.ps f18, f1\n"                                   // opcode=0x5880897b
		"fmul.s f26, f0, f12, dyn\n"                          // opcode=0x10c07d53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m0, m2, m6\n"                                 // opcode=0x6661607b
		"fsrli.pi f16, f1, 0\n"                               // opcode=0x4e10d87b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f26, x15, dyn\n"                            // opcode=0xd007fd53


		"fsw.ps f12, 887(x13)\n"                              // opcode=0x36c6eb8b
		"fnmsub.s f26, f12, f1, f13, dyn\n"                   // opcode=0x68167d4b
		"flw.ps f1,  32 (x4)\n"
		"fmvs.x.ps x25, f15, 3\n"                             // opcode=0xe037acfb


		"fsw.ps f0, -1316(x13)\n"                             // opcode=0xac006e0b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f28, f14\n"                                  // opcode=0x06072e7b
		"fnmadd.ps f21, f13, f13, f14, dyn\n"                 // opcode=0x76d6fadb
		"fcvt.pw.ps f29, f0, dyn\n"                           // opcode=0xc0007efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.s.x f21, x1\n"                                   // opcode=0xf0008ad3
		"fle.pi f24, f0, f0\n"                                // opcode=0xa6000c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x28, f12, dyn\n"                           // opcode=0xc0167e53
		"fsetm.pi m0, f14\n"                                  // opcode=0xe007507b
		"maskxor m3, m1, m6\n"                                // opcode=0x6660c1fb

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fround.ps f27, f0, dyn\n"                            // opcode=0x58107dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f31, f1, f15, dyn\n"                          // opcode=0x10f0ffd3
		"flw.ps f1,  32 (x4)\n"
		"fadd.pi f29, f15, f13\n"                             // opcode=0x06d78efb
		"fcmov.ps f17, f13, f14, f0\n"                        // opcode=0x04e6a8bf
		"fcvt.s.w f20, x12, dyn\n"                            // opcode=0xd0067a53
		"masknot m2, m1\n"                                    // opcode=0x6600a17b
		"mov.m.x m6, x15, 91\n"                               // opcode=0x5767f37b
		"faddi.pi f17, f12, 18\n"                             // opcode=0x0c5608bf
		"fslli.pi f22, f14, 12\n"                             // opcode=0x4f971b7b


		"fswizz.ps f16, f0, 75\n"                             // opcode=0xe720787b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.sn16.ps f25, f13\n"                             // opcode=0xd9968cfb
		"fexp.ps f19, f12\n"                                  // opcode=0x584609fb
		"cubeface.ps f29, f14, f12\n"                         // opcode=0x88c70efb


		"fsq2 f15, 1536(x13)\n"                               // opcode=0x2af5d4a7
		"fmv.s.x f0, x13\n"                                   // opcode=0xf0068053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.ps f28, f0, f1, dyn\n"                          // opcode=0x10107e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fadd.pi f0, f14, f13\n"                              // opcode=0x06d7007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvs.x.ps x22, f0, 6\n"                              // opcode=0xe0602b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m0, m7, m4\n"                                // opcode=0x6643c07b
		"fsra.pi f18, f13, f13\n"                             // opcode=0x0ed6d97b
		"fcvt.pwu.ps f28, f13, dyn\n"                         // opcode=0xc016fe7b
		"for.pi f26, f14, f1\n"                               // opcode=0x06176d7b
		"fxor.pi f22, f13, f1\n"                              // opcode=0x0616cb7b
		"fmvs.x.ps x21, f12, 3\n"                             // opcode=0xe0362afb
		"cubeface.ps f30, f13, f13\n"                         // opcode=0x88d68f7b


		"fbc.ps f19, 1664(x13)\n"                             // opcode=0x7b16098b
		"maskpopcz x26, m1\n"                                 // opcode=0x54008d7b
		"fsgnjx.ps f26, f12, f14\n"                           // opcode=0x20e62d7b
		"fcmov.ps f31, f14, f14, f0\n"                        // opcode=0x04e72fbf
		"cubefaceidx.ps f19, f0, f15\n"                       // opcode=0x88f019fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un8.ps f26, f15\n"                              // opcode=0xd9378d7b
		"flt.ps f29, f13, f15\n"                              // opcode=0xa0f69efb
		"fexp.ps f19, f13\n"                                  // opcode=0x584689fb
		"fltm.ps m5, f0, f1\n"                                // opcode=0xa01052fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f21, f14\n"                                // opcode=0x06173afb
		"fpackrepb.pi f28, f14\n"                             // opcode=0x26070e7b
		"ffrc.ps f0, f14\n"                                   // opcode=0x5827007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f23, 226(x13)\n"                                 // opcode=0x0e20ab87
		"feq.s x18, f0, f15\n"                                // opcode=0xa0f02953
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.pi f17, f1, f0\n"                               // opcode=0x060088fb
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x21\n"                                      // opcode=0xd6000afb


		"fsq2 f12, -1088(x14)\n"                              // opcode=0xdac75027
		"fcvt.ps.f16 f16, f15\n"                              // opcode=0xd0a7887b
		"feq.pi f31, f1, f0\n"                                // opcode=0xa600affb
		"flw.ps f1,  32 (x4)\n"
		"flog.ps f19, f12\n"                                  // opcode=0x583609fb
		"fpackrepb.pi f1, f1\n"                               // opcode=0x260080fb
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x19\n"                                      // opcode=0xd60009fb




		//RTLMIN-5479"frsq.ps f18, f14\n"                                  // opcode=0x5887097b
		"cubefaceidx.ps f16, f0, f1\n"                        // opcode=0x8810187b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.s f18, f1, f15, f1, dyn\n"                     // opcode=0x08f0f943
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0x41a14af4e0341633\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fpackreph.pi f21, f15\n"                             // opcode=0x26079afb
		"fnmadd.ps f27, f1, f13, f1, dyn\n"                   // opcode=0x0ed0fddb
		"flw.ps f1,  32 (x4)\n"




		"cubeface.ps f18, f1, f15\n"                          // opcode=0x88f0897b
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x23, m5\n"                                 // opcode=0x54028bfb
		"fnmsub.ps f26, f0, f12, f1, dyn\n"                   // opcode=0x0cc07d5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsub.ps f17, f1, f12, dyn\n"                         // opcode=0x08c0f8fb
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m4, f12, f12\n"                              // opcode=0xa0c6627b


		"flq2 f23, -928(x12)\n"                               // opcode=0xd870db87


		"fsq2 f15, 960(x14)\n"                                // opcode=0x28f65427
		"fsra.pi f27, f13, f14\n"                             // opcode=0x0ee6ddfb
		"fmv.s.x f27, x11\n"                                  // opcode=0xf0058dd3
		"fcvt.s.w f19, x1, dyn\n"                             // opcode=0xd000f9d3
		"fminu.pi f19, f15, f13\n"                            // opcode=0x2ed7a9fb
		"fcvt.un16.ps f25, f12\n"                             // opcode=0xd9160cfb
		"fandi.pi f24, f15, 92\n"                             // opcode=0x2d979c3f
		"mova.x.m x31\n"                                      // opcode=0xd6000ffb
		"fandi.pi f24, f13, 58\n"                             // opcode=0x1d469c3f
		"fmulhu.pi f27, f1, f0\n"                             // opcode=0x16009dfb
		"flw.ps f1,  32 (x4)\n"




		"feq.ps f17, f15, f15\n"                              // opcode=0xa0f7a8fb
		"fsgnj.s f17, f1, f12\n"                              // opcode=0x20c088d3
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f24, f14, 52\n"                             // opcode=0x1c870c3f
		"fbci.pi f24, 336612\n"                               // opcode=0xa45c8c5f
		"fmsub.ps f29, f12, f13, f13, dyn\n"                  // opcode=0x6ad67edb
		"fmax.ps f31, f14, f12\n"                             // opcode=0x28c71ffb
		"fmax.ps f16, f1, f12\n"                              // opcode=0x28c0987b
		"flw.ps f1,  32 (x4)\n"
		"fpackrepb.pi f23, f12\n"                             // opcode=0x26060bfb
		"fsgnj.ps f23, f15, f13\n"                            // opcode=0x20d78bfb


		"fsw f1, -1084(x12)\n"                                // opcode=0xbc162227
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pwu.ps f20, f12, dyn\n"                         // opcode=0xc0167a7b
		"fcmovm.ps f26, f12, f14\n"                           // opcode=0x00e60d77
		"fltu.pi f28, f1, f13\n"                              // opcode=0xa6d0be7b
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f17, f1, f14, f13, dyn\n"                   // opcode=0x68e0f8db
		"flw.ps f1,  32 (x4)\n"
		"for.pi f30, f15, f1\n"                               // opcode=0x0617ef7b


		"flw.ps f25, -752(x14)\n"                             // opcode=0xd1072c8b
		"fcvt.f16.ps f20, f13\n"                              // opcode=0xd8968a7b
		"fcvt.un8.ps f30, f1\n"                               // opcode=0xd9308f7b
		"fsat8.pi f19, f15\n"                                 // opcode=0x0607b9fb


		"flw.ps f0, 1474(x12)\n"                              // opcode=0x5c27200b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f21, f12, f12\n"                      // opcode=0x88c61afb
		"maskpopcz x0, m7\n"                                  // opcode=0x5403807b
		"fnmsub.s f23, f12, f0, f12, dyn\n"                   // opcode=0x60067bcb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flq2 f27, -576(x12)\n"                               // opcode=0xc1365d87
		"flt.ps f1, f12, f15\n"                               // opcode=0xa0f610fb
		"flw.ps f1,  32 (x4)\n"
		"fexp.ps f23, f13\n"                                  // opcode=0x58468bfb
		"feq.s x19, f1, f12\n"                                // opcode=0xa0c0a9d3
		"flw.ps f1,  32 (x4)\n"
		"feq.s x16, f0, f15\n"                                // opcode=0xa0f02853
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flog.ps f31, f13\n"                                  // opcode=0x58368ffb
		"fclass.s x20, f0\n"                                  // opcode=0xe0001a53
		//RTLMIN-5479"fsin.ps f1, f12\n"                                   // opcode=0x586600fb
		"flw.ps f1,  32 (x4)\n"
		"fmulh.pi f0, f15, f0\n"                              // opcode=0x1607a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fminu.pi f17, f0, f13\n"                             // opcode=0x2ed028fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x17, f14, 3\n"                             // opcode=0xe03708fb
		"fmul.s f20, f1, f0, dyn\n"                           // opcode=0x1000fa53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f29, f15, f1\n"                            // opcode=0x20179efb
		"fcvt.wu.s x16, f13, dyn\n"                           // opcode=0xc016f853
		"fmax.pi f25, f15, f12\n"                             // opcode=0x2ec79cfb
		"fltm.ps m0, f12, f1\n"                               // opcode=0xa016507b


		"fsq2 f15, -608(x15)\n"                               // opcode=0x82f7d1a7
		"flt.s x17, f0, f15\n"                                // opcode=0xa0f018d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x17, f13\n"                                  // opcode=0xe00688d3
		"fmulhu.pi f28, f13, f14\n"                           // opcode=0x16e69e7b
		"fmaxu.pi f16, f15, f13\n"                            // opcode=0x2ed7b87b
		"fadd.s f27, f1, f14, dyn\n"                          // opcode=0x00e0fdd3
		"flw.ps f1,  32 (x4)\n"


		"flw.ps f30, 488(x13)\n"                              // opcode=0x1e87af0b
		"fcvt.ps.pwu f22, f13, dyn\n"                         // opcode=0xd016fb7b
		"for.pi f28, f1, f14\n"                               // opcode=0x06e0ee7b
		"flw.ps f1,  32 (x4)\n"
		"ffrc.ps f16, f14\n"                                  // opcode=0x5827087b
		"cubeface.ps f16, f15, f13\n"                         // opcode=0x88d7887b
		"fclass.ps f27, f0\n"                                 // opcode=0xe0001dfb






		"flw.ps f25, 813(x11)\n"                              // opcode=0x32d0ac8b
		"mov.m.x m5, x14, 41\n"                               // opcode=0x56a732fb
		"mov.m.x m7, x12, 56\n"                               // opcode=0x56e603fb
		"flt.s x23, f12, f1\n"                                // opcode=0xa0161bd3
		"fmin.pi f27, f15, f12\n"                             // opcode=0x2ec78dfb
		"fcvt.s.w f20, x15, dyn\n"                            // opcode=0xd007fa53
		"fcvt.pw.ps f27, f14, dyn\n"                          // opcode=0xc0077dfb
		"fcvt.un16.ps f17, f14\n"                             // opcode=0xd91708fb
		"fsetm.pi m6, f0\n"                                   // opcode=0xe000537b
		"fsgnjx.ps f25, f15, f0\n"                            // opcode=0x2007acfb
		"fadd.s f26, f1, f12, dyn\n"                          // opcode=0x00c0fd53
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f17, 135840\n"                               // opcode=0x4254089f
		"fmulh.pi f31, f15, f13\n"                            // opcode=0x16d7affb
		"fmv.s.x f20, x1\n"                                   // opcode=0xf0008a53
		"fmv.s.x f22, x1\n"                                   // opcode=0xf0008b53
		"fsat8.pi f16, f12\n"                                 // opcode=0x0606387b
		"fexp.ps f21, f0\n"                                   // opcode=0x58400afb
		"fcvt.pwu.ps f26, f14, dyn\n"                         // opcode=0xc0177d7b
		"flt.s x17, f13, f0\n"                                // opcode=0xa00698d3
		"flt.ps f20, f12, f1\n"                               // opcode=0xa0161a7b
		"fcvt.sn8.ps f24, f15\n"                              // opcode=0xd9b78c7b
		"feq.s x19, f14, f0\n"                                // opcode=0xa00729d3
		//RTLMIN-5479"frsq.ps f23, f15\n"                                  // opcode=0x58878bfb
		"fsgnjn.s f22, f14, f15\n"                            // opcode=0x20f71b53
		"fsrl.pi f23, f0, f13\n"                              // opcode=0x06d05bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f31, f12, f14\n"                             // opcode=0x0ee65ffb




		"fcvt.s.w f18, x15, dyn\n"                            // opcode=0xd007f953
		"fround.ps f23, f14, dyn\n"                           // opcode=0x58177bfb
		"fsetm.pi m6, f13\n"                                  // opcode=0xe006d37b
		"fle.s x16, f13, f13\n"                               // opcode=0xa0d68853
		"fsrai.pi f26, f1, 8\n"                               // opcode=0x4f00fd7b
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xbe421838c4a6fc26\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs





		"fadd.pi f23, f0, f15\n"                              // opcode=0x06f00bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.un10.ps f21, f0\n"                              // opcode=0xd9200afb


		"flw f0, -334(x12)\n"                                 // opcode=0xeb262007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x0\n"                                       // opcode=0xd600107b


		"flq2 f22, 64(x13)\n"                                 // opcode=0x41f05b07
		"fcvt.f16.ps f31, f1\n"                               // opcode=0xd8908ffb
		"fle.pi f22, f14, f1\n"                               // opcode=0xa6170b7b


		"fbc.ps f0, 832(x13)\n"                               // opcode=0x22a7800b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.pi f0, f12, f13\n"                               // opcode=0xa6d6207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x21, m6\n"                                  // opcode=0x52030afb
		"faddi.pi f26, f14, 183\n"                            // opcode=0x5cf70d3f
		"fcvt.ps.pw f18, f14, dyn\n"                          // opcode=0xd007797b
		"mova.m.x x0\n"                                       // opcode=0xd600107b
		"fle.pi f22, f13, f1\n"                               // opcode=0xa6168b7b


		"fmvs.x.ps x30, f15, 0\n"                             // opcode=0xe007af7b


		"flw.ps f25, -1701(x11)\n"                            // opcode=0x95b5ac8b
		"flem.ps m1, f14, f1\n"                               // opcode=0xa01740fb
		"fcvt.wu.s x19, f12, dyn\n"                           // opcode=0xc01679d3
		"fmax.pi f26, f1, f0\n"                               // opcode=0x2e009d7b
		"flw.ps f1,  32 (x4)\n"




		"mov.m.x m7, x0, 122\n"                               // opcode=0x57e043fb
		"fltm.pi m0, f15, f15\n"                              // opcode=0x3ef7807b
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fmvs.x.ps x31, f14, 1\n"                             // opcode=0xe0172ffb
		"fsgnj.s f0, f12, f1\n"                               // opcode=0x20160053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fxor.pi f29, f14, f15\n"                             // opcode=0x06f74efb


		"fsq2 f14, -1728(x15)\n"                              // opcode=0xb4e7d5a7
		"fcvt.w.s x25, f1, dyn\n"                             // opcode=0xc000fcd3
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x0\n"                                       // opcode=0xd600107b
		"fnot.pi f25, f14\n"                                  // opcode=0x06072cfb
		"flem.ps m7, f1, f0\n"                                // opcode=0xa000c3fb
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f0, f13, f14\n"                               // opcode=0x28e68053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f29, f13, f0\n"                              // opcode=0x0e06defb
		"feqm.ps m2, f1, f0\n"                                // opcode=0xa000e17b
		"flw.ps f1,  32 (x4)\n"
		"flem.ps m5, f1, f15\n"                               // opcode=0xa0f0c2fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un8.ps f20, f12\n"                              // opcode=0xd9360a7b
		"fle.ps f24, f14, f0\n"                               // opcode=0xa0070c7b
		"fmin.s f0, f12, f15\n"                               // opcode=0x28f60053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fand.pi f22, f13, f1\n"                              // opcode=0x0616fb7b
		"fcmov.ps f24, f1, f12, f1\n"                         // opcode=0x0cc0ac3f
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x22, f0\n"                                   // opcode=0xe0000b53
		"fcvt.un24.ps f28, f0\n"                              // opcode=0xd9000e7b
		"fnmsub.ps f17, f15, f14, f12, dyn\n"                 // opcode=0x64e7f8db
		"fsub.pi f19, f14, f15\n"                             // opcode=0x0ef709fb
		"fxor.pi f18, f14, f14\n"                             // opcode=0x06e7497b
		"fsgnjx.s f30, f15, f15\n"                            // opcode=0x20f7af53
		"maskpopc x1, m1\n"                                   // opcode=0x520080fb
		"fle.ps f19, f0, f0\n"                                // opcode=0xa00009fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m5, f1, f0\n"                                // opcode=0xa000e2fb
		"flw.ps f1,  32 (x4)\n"
		"fround.ps f27, f13, dyn\n"                           // opcode=0x5816fdfb
		"fmvs.x.ps x20, f14, 2\n"                             // opcode=0xe0272a7b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.un2.ps f20, f15\n"                              // opcode=0xd9778a7b


		"fsq2 f15, 1984(x15)\n"                               // opcode=0x3af0daa7
		"flt.ps f1, f13, f13\n"                               // opcode=0xa0d690fb
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f23, f13, f0, f13, rup\n"                   // opcode=0x6a06bbdb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f0, f1, dyn\n"                            // opcode=0xd000f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmul.ps f25, f1, f0, dyn\n"                          // opcode=0x1000fcfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f21, f1, f12\n"                               // opcode=0xa0c08afb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pwu.ps f0, f0, rtz\n"                           // opcode=0xc010107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.s f30, f15, f14\n"                              // opcode=0x28e79f53
		"fmax.pi f17, f12, f15\n"                             // opcode=0x2ef618fb
		"fsrl.pi f29, f13, f15\n"                             // opcode=0x06f6defb
		"fcvt.pwu.ps f19, f14, dyn\n"                         // opcode=0xc01779fb
		"fsrai.pi f25, f14, 14\n"                             // opcode=0x4fc77cfb
		"cubeface.ps f17, f0, f12\n"                          // opcode=0x88c008fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f20, f13, 2\n"                              // opcode=0x4e56fa7b
		"fround.ps f30, f13, dyn\n"                           // opcode=0x5816ff7b
		"for.pi f18, f12, f1\n"                               // opcode=0x0616697b


		"fsw.ps f1, 1265(x11)\n"                              // opcode=0x4e15e88b
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x18, m5\n"                                 // opcode=0x5402897b
		"mova.x.m x18\n"                                      // opcode=0xd600097b
		"fsll.pi f22, f0, f15\n"                              // opcode=0x06f01b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fminu.pi f1, f14, f12\n"                             // opcode=0x2ec720fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f23, f12\n"                              // opcode=0xd8960bfb


		"feq.s x21, f14, f0\n"                                // opcode=0xa0072ad3
		"fcvt.un16.ps f25, f12\n"                             // opcode=0xd9160cfb


		"flw f0, 1350(x11)\n"                                 // opcode=0x5460a007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un2.ps f28, f13\n"                              // opcode=0xd9768e7b




		"fsgnj.s f24, f13, f13\n"                             // opcode=0x20d68c53
		"fltm.ps m0, f12, f12\n"                              // opcode=0xa0c6507b
		"fcvt.un16.ps f31, f13\n"                             // opcode=0xd9168ffb
		"maskxor m5, m1, m3\n"                                // opcode=0x6630c2fb
		"fcvt.sn8.ps f17, f15\n"                              // opcode=0xd9b788fb
		"cubefaceidx.ps f20, f14, f15\n"                      // opcode=0x88f71a7b
		//RTLMIN-5479"fsin.ps f26, f14\n"                                  // opcode=0x58670d7b
		"fcvt.s.w f28, x13, dyn\n"                            // opcode=0xd006fe53
		"fcvt.un16.ps f28, f15\n"                             // opcode=0xd9178e7b
		"maskpopc x1, m4\n"                                   // opcode=0x520200fb


		"fbc.ps f22, 608(x11)\n"                              // opcode=0x06858b0b
		"mova.m.x x15\n"                                      // opcode=0xd607907b
		"fsub.ps f24, f12, f14, rdn\n"                        // opcode=0x08e62c7b
		"fcvt.pwu.ps f24, f1, dyn\n"                          // opcode=0xc010fc7b
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f29, f12\n"                                // opcode=0xe0061efb
		"fmax.s f16, f0, f15\n"                               // opcode=0x28f01853
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.ps f23, f1, f1\n"                                // opcode=0xa0108bfb
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"fsin.ps f18, f15\n"                                  // opcode=0x5867897b
		"fltm.ps m4, f1, f13\n"                               // opcode=0xa0d0d27b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x26, f1, dyn\n"                             // opcode=0xc000fd53
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f1, f12, f15\n"                               // opcode=0xa6f600fb
		"flw.ps f1,  32 (x4)\n"
		"fbci.pi f23, 287710\n"                               // opcode=0x8c7bcbdf
		"fsgnjn.ps f31, f13, f13\n"                           // opcode=0x20d69ffb

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fnmadd.s f31, f13, f13, f12, dyn\n"                  // opcode=0x60d6ffcf
		//RTLMIN-5479"fsin.ps f25, f0\n"                                   // opcode=0x58600cfb
		"fbcx.ps f22, x12\n"                                  // opcode=0x00063b0b
		"maskor m5, m1, m3\n"                                 // opcode=0x6630e2fb
		"fmul.s f1, f14, f13, dyn\n"                          // opcode=0x10d770d3
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m6, f12, f1\n"                               // opcode=0xa016637b


		"fbc.ps f19, -1056(x11)\n"                            // opcode=0xf3b5898b
		"fpackrepb.pi f1, f12\n"                              // opcode=0x260600fb
		"flw.ps f1,  32 (x4)\n"




		"fmul.s f0, f12, f14, dyn\n"                          // opcode=0x10e67053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f0, f15, f0\n"                               // opcode=0xa607b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvs.x.ps x30, f0, 0\n"                              // opcode=0xe0002f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un24.ps f28, f14\n"                             // opcode=0xd9070e7b
		"masknot m4, m2\n"                                    // opcode=0x6601227b


		"flw f27, 1902(x14)\n"                                // opcode=0x76e7ad87
		"fbci.pi f18, 46529\n"                                // opcode=0x16b8395f
		"fexp.ps f19, f14\n"                                  // opcode=0x584709fb
		"fltu.pi f22, f13, f0\n"                              // opcode=0xa606bb7b
		"fcvt.w.s x24, f1, dyn\n"                             // opcode=0xc000fc53
		"flw.ps f1,  32 (x4)\n"
		"fbci.pi f25, 168037\n"                               // opcode=0x520cacdf
		"fsub.s f22, f14, f0, dyn\n"                          // opcode=0x08077b53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f23, 923(x15)\n"                                 // opcode=0x39b02b87
		"fcvt.wu.s x21, f1, dyn\n"                            // opcode=0xc010fad3
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f1, f13, 49\n"                             // opcode=0xe6c6b0fb
		"flw.ps f1,  32 (x4)\n"


		"flw f23, -667(x12)\n"                                // opcode=0xd6562b87
		"fmin.pi f0, f0, f12\n"                               // opcode=0x2ec0007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f21, f0, dyn\n"                          // opcode=0xd0107afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f22, 200329\n"                               // opcode=0x61d12b5f
		"fcvt.un16.ps f27, f1\n"                              // opcode=0xd9108dfb
		"fsub.s f23, f15, f0, rmm\n"                          // opcode=0x0807cbd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f28, x0, dyn\n"                             // opcode=0xd0007e53
		"fclass.ps f21, f1\n"                                 // opcode=0xe0009afb
		"fsub.pi f29, f15, f14\n"                             // opcode=0x0ee78efb
		"fcvt.un8.ps f20, f15\n"                              // opcode=0xd9378a7b
		"ffrc.ps f0, f0\n"                                    // opcode=0x5820007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x31, f12, 3\n"                             // opcode=0xe0360ffb
		"fmv.x.s x1, f14\n"                                   // opcode=0xe00700d3
		"fsat8.pi f18, f12\n"                                 // opcode=0x0606397b
		"masknot m1, m2\n"                                    // opcode=0x660120fb
		"fmul.ps f20, f0, f13, dyn\n"                         // opcode=0x10d07a7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f1, f12, f13\n"                              // opcode=0x2ed600fb
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f28, f13, 2\n"                              // opcode=0x4e56de7b
		"fxor.pi f31, f14, f1\n"                              // opcode=0x06174ffb
		"fcvt.s.wu f20, x13, dyn\n"                           // opcode=0xd016fa53


		"fbc.ps f27, 1696(x11)\n"                             // opcode=0x0eb00d8b


		"flw f28, 1278(x14)\n"                                // opcode=0x4fe62e07
		"fcmov.ps f30, f13, f13, f15\n"                       // opcode=0x7cd6af3f
		"fsgnj.ps f20, f0, f15\n"                             // opcode=0x20f00a7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m0, x0, 7\n"                                 // opcode=0x5610707b




		"fcvt.ps.pwu f22, f0, dyn\n"                          // opcode=0xd0107b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f16, f15, f14\n"                             // opcode=0x0ee7887b

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsub.s f21, f15, f13, dyn\n"                         // opcode=0x08d7fad3
		"fadd.ps f0, f1, f15, dyn\n"                          // opcode=0x00f0f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"flw f0, 1702(x11)\n"                                 // opcode=0x6a672007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f19, f0\n"                                 // opcode=0x061039fb
		"flt.ps f27, f14, f12\n"                              // opcode=0xa0c71dfb
		"fcvt.sn8.ps f21, f13\n"                              // opcode=0xd9b68afb
		"fadd.pi f0, f12, f13\n"                              // opcode=0x06d6007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.pi f17, f13, f0\n"                              // opcode=0x160688fb
		"fmin.ps f24, f0, f12\n"                              // opcode=0x28c00c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.s.x f22, x11\n"                                  // opcode=0xf0058b53
		"fsgnj.ps f28, f12, f14\n"                            // opcode=0x20e60e7b
		"fmv.x.s x21, f12\n"                                  // opcode=0xe0060ad3
		"flt.pi f25, f15, f12\n"                              // opcode=0xa6c79cfb
		"cubeface.ps f27, f13, f1\n"                          // opcode=0x88168dfb
		"fmul.s f19, f14, f12, dyn\n"                         // opcode=0x10c779d3
		"flt.ps f25, f15, f14\n"                              // opcode=0xa0e79cfb
		//RTLMIN-5479"fsin.ps f23, f1\n"                                   // opcode=0x58608bfb
		"maskand m3, m5, m7\n"                                // opcode=0x6672f1fb
		"fsgnjn.ps f0, f12, f15\n"                            // opcode=0x20f6107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x22, f13\n"                                  // opcode=0xe0068b53
		"fle.ps f26, f1, f1\n"                                // opcode=0xa0108d7b
		"flw.ps f1,  32 (x4)\n"
		"fmvz.x.ps x24, f13, 7\n"                             // opcode=0xe0768c7b




		"fcvt.pw.ps f29, f0, dyn\n"                           // opcode=0xc0007efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f19, 1716(x15)\n"                                // opcode=0x6b40a987


		"fmax.ps f30, f0, f14\n"                              // opcode=0x28e01f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f24, f15, 13\n"                             // opcode=0x4fb7fc7b
		"fcvt.ps.f16 f21, f14\n"                              // opcode=0xd0a70afb
		"fcvt.un2.ps f20, f13\n"                              // opcode=0xd9768a7b
		"fmaxu.pi f22, f1, f15\n"                             // opcode=0x2ef0bb7b
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m6, f0\n"                                   // opcode=0xe000537b
		"fsgnjn.s f22, f13, f0\n"                             // opcode=0x20069b53
		"fclass.ps f20, f1\n"                                 // opcode=0xe0009a7b
		"fsatu8.pi f25, f0\n"                                 // opcode=0x06103cfb
		"fcvt.w.s x17, f15, rmm\n"                            // opcode=0xc007c8d3
		"fsetm.pi m3, f15\n"                                  // opcode=0xe007d1fb
		"fnmadd.ps f18, f1, f13, f1, dyn\n"                   // opcode=0x0ed0f95b
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f29, f15, f12\n"                             // opcode=0x06c7cefb


		"fsw.ps f0, -1062(x13)\n"                             // opcode=0xbc006d0b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f0, -127(x14)\n"                              // opcode=0xf807608b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f29, f15, f0\n"                              // opcode=0x20078ed3
		"fcvt.un10.ps f22, f1\n"                              // opcode=0xd9208b7b


		"flw f0, 1994(x14)\n"                                 // opcode=0x7ca5a007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.s x18, f1\n"                                  // opcode=0xe0009953
		"feq.s x30, f14, f14\n"                               // opcode=0xa0e72f53
		"fmul.s f21, f14, f0, dyn\n"                          // opcode=0x10077ad3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f18, f13, f1\n"                             // opcode=0x20169953
		"fcvt.un24.ps f1, f15\n"                              // opcode=0xd90780fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un16.ps f26, f15\n"                             // opcode=0xd9178d7b
		"feqm.ps m0, f13, f0\n"                               // opcode=0xa006e07b
		"cubefaceidx.ps f28, f0, f12\n"                       // opcode=0x88c01e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xfbf1c85993996bd9\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fandi.pi f16, f15, 331\n"                            // opcode=0xa567983f
		"cubesgnsc.ps f19, f13, f0\n"                         // opcode=0x8806a9fb
		"fmul.pi f1, f14, f0\n"                               // opcode=0x160700fb
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f0, 822(x13)\n"                               // opcode=0x3206eb0b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fminu.pi f27, f15, f1\n"                             // opcode=0x2e17adfb
		"cubesgntc.ps f30, f12, f0\n"                         // opcode=0x88063f7b
		"feq.pi f30, f13, f0\n"                               // opcode=0xa606af7b


		"fsw.ps f14, -481(x15)\n"                             // opcode=0xe0e7ef8b
		"fbci.pi f0, 96121\n"                                 // opcode=0x2eef205f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsat8.pi f18, f14\n"                                 // opcode=0x0607397b
		"fcmovm.ps f22, f0, f13\n"                            // opcode=0x00d00b77
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.f16 f26, f15\n"                              // opcode=0xd0a78d7b
		"faddi.pi f28, f0, 106\n"                             // opcode=0x35500e3f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m4, f1, f13\n"                               // opcode=0x3ed0827b
		"flw.ps f1,  32 (x4)\n"
		"fmax.ps f27, f14, f15\n"                             // opcode=0x28f71dfb


		"fsq2 f1, -32(x11)\n"                                 // opcode=0xfe15d427
		"flw.ps f1,  32 (x4)\n"


		"fsw f0, -664(x13)\n"                                 // opcode=0xd606a427
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x31, m0\n"                                  // opcode=0x52000ffb
		"fcvt.un8.ps f31, f0\n"                               // opcode=0xd9300ffb
		"fsub.s f17, f1, f14, dyn\n"                          // opcode=0x08e0f8d3
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x16\n"                                      // opcode=0xd600087b
		"fmin.s f0, f14, f0\n"                                // opcode=0x28070053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fmulh.pi f31, f14, f1\n"                             // opcode=0x16172ffb
		"fsgnjn.ps f20, f15, f12\n"                           // opcode=0x20c79a7b
		"fsgnj.s f22, f0, f0\n"                               // opcode=0x20000b53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fexp.ps f31, f14\n"                                  // opcode=0x58470ffb
		"fmadd.ps f21, f12, f12, f0, dyn\n"                   // opcode=0x00c67adb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f20, f13\n"                             // opcode=0x26069a7b
		"fbcx.ps f27, x0\n"                                   // opcode=0x00003d8b
		"fbci.pi f18, 232370\n"                               // opcode=0x7176495f
		"fmulh.pi f17, f15, f14\n"                            // opcode=0x16e7a8fb
		"flt.ps f27, f1, f1\n"                                // opcode=0xa0109dfb
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f29, f1, f15\n"                              // opcode=0xa6f0befb
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f26, f12, f12\n"                              // opcode=0xa0c60d7b
		"fltm.ps m4, f15, f14\n"                              // opcode=0xa0e7d27b
		"fmin.ps f19, f15, f14\n"                             // opcode=0x28e789fb
		"maskxor m4, m7, m3\n"                                // opcode=0x6633c27b
		"fcvt.ps.pw f19, f15, dyn\n"                          // opcode=0xd007f9fb
		"fsgnjx.ps f31, f1, f13\n"                            // opcode=0x20d0affb
		"flw.ps f1,  32 (x4)\n"


		"flw.ps f0, -1078(x13)\n"                             // opcode=0xbca0a00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"feqm.ps m5, f13, f14\n"                              // opcode=0xa0e6e2fb
		//RTLMIN-5479"frsq.ps f29, f0\n"                                   // opcode=0x58800efb
		"fmv.s.x f18, x13\n"                                  // opcode=0xf0068953






		"frcp.ps f24, f1\n"                                   // opcode=0x58708c7b
		"fltm.pi m3, f14, f0\n"                               // opcode=0x3e0701fb
		"fsrli.pi f23, f1, 9\n"                               // opcode=0x4f30dbfb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un2.ps f26, f15\n"                              // opcode=0xd9778d7b
		"maskor m4, m6, m0\n"                                 // opcode=0x6603627b
		"fmvs.x.ps x31, f14, 0\n"                             // opcode=0xe0072ffb
		"fsgnjx.ps f21, f12, f13\n"                           // opcode=0x20d62afb

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fexp.ps f19, f14\n"                                  // opcode=0x584709fb
		"fnmadd.ps f17, f12, f1, f14, dyn\n"                  // opcode=0x761678db
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f17, f1\n"                                  // opcode=0x0600b8fb
		"fcmovm.ps f19, f0, f12\n"                            // opcode=0x00c009f7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.ps f17, f0, f0, f1, dyn\n"                     // opcode=0x0a0078db
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f20, f0, f0\n"                              // opcode=0x20002a53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f28, f13, 112\n"                           // opcode=0xe7c69e7b
		"fsgnjx.s f24, f14, f14\n"                            // opcode=0x20e72c53
		"fltm.ps m6, f15, f15\n"                              // opcode=0xa0f7d37b
		"fcmov.ps f25, f0, f0, f1\n"                          // opcode=0x0c002cbf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.ps f25, f12, f1\n"                               // opcode=0xa0160cfb
		"masknot m7, m7\n"                                    // opcode=0x6603a3fb
		"fsgnjx.ps f27, f1, f0\n"                             // opcode=0x2000adfb
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f23, f12\n"                                // opcode=0x06163bfb
		"fltm.pi m7, f0, f12\n"                               // opcode=0x3ec003fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsll.pi f20, f13, f13\n"                             // opcode=0x06d69a7b
		"fle.pi f19, f1, f1\n"                                // opcode=0xa61089fb
		"flw.ps f1,  32 (x4)\n"
		"feq.pi f23, f14, f12\n"                              // opcode=0xa6c72bfb


		"fnmsub.ps f28, f15, f15, f12, dyn\n"                 // opcode=0x64f7fe5b
		"fmul.s f20, f15, f15, dyn\n"                         // opcode=0x10f7fa53
		"fmin.pi f30, f13, f15\n"                             // opcode=0x2ef68f7b


		"fbci.ps f27, 318985\n"                               // opcode=0x9bc12d9f


		"flq2 f30, -416(x11)\n"                               // opcode=0xaa005f07
		"maskand m2, m1, m7\n"                                // opcode=0x6670f17b
		"maskand m5, m6, m0\n"                                // opcode=0x660372fb








		"maskand m1, m1, m2\n"                                // opcode=0x6620f0fb
		"fsgnjn.ps f25, f1, f12\n"                            // opcode=0x20c09cfb
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f28, f12, f15, f14, dyn\n"                   // opcode=0x70f67e47
		"maskpopcz x28, m1\n"                                 // opcode=0x54008e7b




		"fbci.ps f1, 229806\n"                                // opcode=0x7035d09f
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f28, f12, f13\n"                         // opcode=0x88d60e7b
		"fadd.pi f28, f15, f12\n"                             // opcode=0x06c78e7b
		"faddi.pi f18, f1, 432\n"                             // opcode=0xdc10893f
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m4, x0, 111\n"                               // opcode=0x57b0727b
		"flt.s x16, f0, f15\n"                                // opcode=0xa0f01853
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f27, x1, dyn\n"                             // opcode=0xd000fdd3
		"fslli.pi f16, f14, 13\n"                             // opcode=0x4fa7187b


		"fsgnjn.s f25, f14, f15\n"                            // opcode=0x20f71cd3


		"fsw f15, -509(x15)\n"                                // opcode=0xe0f0a1a7
		"fsrl.pi f20, f14, f14\n"                             // opcode=0x06e75a7b
		"fpackrepb.pi f24, f14\n"                             // opcode=0x26070c7b
		"fsat8.pi f23, f1\n"                                  // opcode=0x0600bbfb
		"feq.pi f19, f13, f12\n"                              // opcode=0xa6c6a9fb
		"fmvs.x.ps x19, f0, 0\n"                              // opcode=0xe00029fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.s x29, f13, f13\n"                               // opcode=0xa0d69ed3
		"fexp.ps f25, f13\n"                                  // opcode=0x58468cfb
		"faddi.pi f20, f1, 270\n"                             // opcode=0x85c08a3f
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f20, f13, f15\n"                            // opcode=0x20f68a7b
		"cubefaceidx.ps f31, f14, f1\n"                       // opcode=0x88171ffb


		"flq2 f25, 1632(x12)\n"                               // opcode=0x1566dc87

		"li x5, 0xfff5d17e75150633\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fltu.pi f21, f12, f12\n"                             // opcode=0xa6c63afb
		"cubesgnsc.ps f27, f12, f14\n"                        // opcode=0x88e62dfb


		"flw f24, -468(x13)\n"                                // opcode=0xe2c6ac07
		"fbcx.ps f18, x15\n"                                  // opcode=0x0007b90b
		"cubesgntc.ps f17, f15, f12\n"                        // opcode=0x88c7b8fb




		"fslli.pi f1, f0, 1\n"                                // opcode=0x4e3010fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m5, f12, f13\n"                              // opcode=0xa0d662fb
		"flog.ps f26, f14\n"                                  // opcode=0x58370d7b
		"fcvt.wu.s x19, f13, dyn\n"                           // opcode=0xc016f9d3
		"maskxor m6, m7, m3\n"                                // opcode=0x6633c37b
		"fcvt.s.wu f29, x15, dyn\n"                           // opcode=0xd017fed3
		"fmv.s.x f18, x14\n"                                  // opcode=0xf0070953
		"fmv.x.s x1, f0\n"                                    // opcode=0xe00000d3
		"fltu.pi f25, f14, f15\n"                             // opcode=0xa6f73cfb
		"fmin.pi f0, f12, f14\n"                              // opcode=0x2ee6007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.s x20, f15, f1\n"                                // opcode=0xa0178a53


		"flw f26, 1901(x14)\n"                                // opcode=0x76d5ad07
		"fandi.pi f27, f15, 215\n"                            // opcode=0x6ce79dbf
		"flt.ps f26, f0, f12\n"                               // opcode=0xa0c01d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.ps f21, f13, f14\n"                              // opcode=0xa0e69afb
		"fmax.s f20, f0, f15\n"                               // opcode=0x28f01a53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.ps f18, f0, f1, dyn\n"                          // opcode=0x1010797b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un8.ps f17, f14\n"                              // opcode=0xd93708fb
		"maskand m2, m5, m4\n"                                // opcode=0x6642f17b
		"feqm.ps m4, f12, f0\n"                               // opcode=0xa006627b
		"fpackreph.pi f24, f15\n"                             // opcode=0x26079c7b




		"fle.pi f24, f13, f1\n"                               // opcode=0xa6168c7b
		"fmvz.x.ps x17, f12, 3\n"                             // opcode=0xe03608fb








		"fadd.ps f27, f12, f0, dyn\n"                         // opcode=0x00067dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fmulh.pi f0, f14, f12\n"                             // opcode=0x16c7207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f17, f0, dyn\n"                          // opcode=0xd01078fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m3, x12, 65\n"                               // opcode=0x570621fb
		"feq.pi f19, f15, f14\n"                              // opcode=0xa6e7a9fb
		"fcvt.wu.s x21, f14, rne\n"                           // opcode=0xc0170ad3
		"fminu.pi f26, f0, f1\n"                              // opcode=0x2e102d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x29, f0\n"                                   // opcode=0xe0000ed3
		"fsrli.pi f31, f15, 10\n"                             // opcode=0x4f47dffb
		"cubesgnsc.ps f1, f15, f0\n"                          // opcode=0x8807a0fb
		"flw.ps f1,  32 (x4)\n"
		"fpackreph.pi f17, f12\n"                             // opcode=0x260618fb
		"fsgnjn.s f30, f14, f0\n"                             // opcode=0x20071f53
		"fmadd.s f28, f1, f0, f1, dyn\n"                      // opcode=0x0800fe43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f24, f14, f1\n"                               // opcode=0xa6170c7b
		"fadd.pi f24, f1, f1\n"                               // opcode=0x06108c7b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.f16 f26, f15\n"                              // opcode=0xd0a78d7b


		"flq2 f28, -224(x11)\n"                               // opcode=0xeef5de07
		"fcvt.w.s x0, f14, dyn\n"                             // opcode=0xc0077053
		"masknot m1, m5\n"                                    // opcode=0x6602a0fb
		"fsetm.pi m6, f12\n"                                  // opcode=0xe006537b
		"flog.ps f31, f15\n"                                  // opcode=0x58378ffb
		"fmin.pi f17, f12, f14\n"                             // opcode=0x2ee608fb

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"mova.x.m x0\n"                                       // opcode=0xd600007b
		"for.pi f17, f1, f12\n"                               // opcode=0x06c0e8fb
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f31, f15, f14\n"                             // opcode=0xa6e7bffb


		"flw.ps f0, 1594(x14)\n"                              // opcode=0x63a0200b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.s f28, f14, f0\n"                               // opcode=0x28071e53
		"fsat8.pi f17, f14\n"                                 // opcode=0x060738fb


		"fsq2 f13, 1376(x14)\n"                               // opcode=0x10d05527
		"flt.s x25, f12, f12\n"                               // opcode=0xa0c61cd3
		"fcvt.f16.ps f28, f13\n"                              // opcode=0xd8968e7b
		"cubesgntc.ps f0, f12, f0\n"                          // opcode=0x8806307b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f25, f1, f1\n"                               // opcode=0x0e10dcfb
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x26\n"                                      // opcode=0xd6000d7b
		"for.pi f23, f14, f15\n"                              // opcode=0x06f76bfb
		"fsgnjx.ps f17, f0, f14\n"                            // opcode=0x20e028fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f21, f14, f13, f1, dyn\n"                    // opcode=0x08d77ac7
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f23, f14, f14\n"                              // opcode=0xa6e71bfb
		"fnot.pi f25, f12\n"                                  // opcode=0x06062cfb
		"fcvt.w.s x25, f12, rtz\n"                            // opcode=0xc0061cd3
		"fcvt.s.wu f24, x15, rtz\n"                           // opcode=0xd0179c53
		"fslli.pi f29, f15, 12\n"                             // opcode=0x4f879efb
		"mov.m.x m5, x13, 27\n"                               // opcode=0x5666e2fb
		"fmvz.x.ps x31, f12, 1\n"                             // opcode=0xe0160ffb
		"fsub.pi f21, f14, f12\n"                             // opcode=0x0ec70afb
		"flog.ps f1, f15\n"                                   // opcode=0x583780fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un24.ps f28, f14\n"                             // opcode=0xd9070e7b
		"fle.s x16, f14, f14\n"                               // opcode=0xa0e70853


		"flw f19, -380(x14)\n"                                // opcode=0xe8472987
		"fclass.s x26, f0\n"                                  // opcode=0xe0001d53
		"fcvt.sn8.ps f23, f1\n"                               // opcode=0xd9b08bfb
		"fcvt.un24.ps f30, f14\n"                             // opcode=0xd9070f7b
		"fmul.ps f25, f15, f0, dyn\n"                         // opcode=0x1007fcfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f30, f12, f15\n"                             // opcode=0x28f60f7b
		"fadd.pi f24, f1, f14\n"                              // opcode=0x06e08c7b
		"flw.ps f1,  32 (x4)\n"
		"flog.ps f22, f15\n"                                  // opcode=0x58378b7b
		"flt.ps f22, f1, f12\n"                               // opcode=0xa0c09b7b
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m5, x15, 74\n"                               // opcode=0x5727d2fb
		"for.pi f1, f15, f13\n"                               // opcode=0x06d7e0fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f21, x15, dyn\n"                           // opcode=0xd017fad3


		"fswizz.ps f22, f12, 54\n"                            // opcode=0xe6d64b7b
		"fle.s x27, f15, f13\n"                               // opcode=0xa0d78dd3


		"flw f21, 2011(x12)\n"                                // opcode=0x7db0aa87
		"fpackreph.pi f0, f0\n"                               // opcode=0x2600107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f24, f14, f14\n"                              // opcode=0x06e76c7b
		"fcvt.w.s x28, f12, dyn\n"                            // opcode=0xc0067e53
		"fsub.s f17, f15, f15, dyn\n"                         // opcode=0x08f7f8d3
		"fle.ps f18, f1, f14\n"                               // opcode=0xa0e0897b
		"flw.ps f1,  32 (x4)\n"
		"fmax.pi f1, f12, f0\n"                               // opcode=0x2e0610fb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f30, f13, f0\n"                             // opcode=0x2006af53




		"fexp.ps f17, f14\n"                                  // opcode=0x584708fb
		"fbci.pi f18, 456421\n"                               // opcode=0xdedcb95f

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"feqm.ps m2, f14, f1\n"                               // opcode=0xa017617b
		"feq.s x1, f13, f12\n"                                // opcode=0xa0c6a0d3
		"flog.ps f29, f14\n"                                  // opcode=0x58370efb
		"fcvt.s.w f31, x12, dyn\n"                            // opcode=0xd0067fd3
		"fbcx.ps f19, x11\n"                                  // opcode=0x0005b98b
		"frcp.ps f18, f13\n"                                  // opcode=0x5876897b
		"fbci.ps f20, 127150\n"                               // opcode=0x3e15da1f
		"fnmsub.s f22, f15, f0, f0, dyn\n"                    // opcode=0x0007fb4b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x30, m4\n"                                 // opcode=0x54020f7b


		"fnmadd.s f31, f1, f15, f1, dyn\n"                    // opcode=0x08f0ffcf
		"flw.ps f1,  32 (x4)\n"
		"maskor m5, m1, m5\n"                                 // opcode=0x6650e2fb
		"flt.pi f18, f13, f14\n"                              // opcode=0xa6e6997b
		"fcvt.sn8.ps f21, f14\n"                              // opcode=0xd9b70afb
		"fcvt.w.s x26, f13, dyn\n"                            // opcode=0xc006fd53
		"fand.pi f19, f0, f13\n"                              // opcode=0x06d079fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackrepb.pi f17, f1\n"                              // opcode=0x260088fb


		"flw f1, -1125(x14)\n"                                // opcode=0xb9b0a087
		"flw.ps f1,  32 (x4)\n"
		"fnot.pi f21, f13\n"                                  // opcode=0x0606aafb
		"fcvt.un2.ps f0, f15\n"                               // opcode=0xd977807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f26, f1, f15\n"                             // opcode=0x16f0ad7b
		"flw.ps f1,  32 (x4)\n"


		"fsw f13, 208(x15)\n"                                 // opcode=0x0cd0a827
		"maskand m3, m7, m0\n"                                // opcode=0x6603f1fb


		"flw.ps f23, -1127(x13)\n"                            // opcode=0xb996ab8b
		"fround.ps f23, f15, dyn\n"                           // opcode=0x5817fbfb
		"fcvt.pwu.ps f24, f14, dyn\n"                         // opcode=0xc0177c7b


		"fbc.ps f28, -1120(x13)\n"                            // opcode=0x96368e0b
		"mova.m.x x0\n"                                       // opcode=0xd600107b
		"fsgnjn.ps f20, f1, f1\n"                             // opcode=0x20109a7b
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m7, f13\n"                                  // opcode=0xe006d3fb
		"fsrli.pi f1, f14, 2\n"                               // opcode=0x4e4750fb
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f19, f15, f14\n"                            // opcode=0x2ee7b9fb
		"fnmsub.s f23, f1, f15, f12, dyn\n"                   // opcode=0x60f0fbcb
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m6, x15, 79\n"                               // opcode=0x5737e37b
		"mova.m.x x13\n"                                      // opcode=0xd606907b
		"fle.pi f23, f12, f15\n"                              // opcode=0xa6f60bfb
		"fcmov.ps f17, f1, f1, f13\n"                         // opcode=0x6c10a8bf
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f31, f15, f15\n"                           // opcode=0x00f78ff7
		"fmadd.ps f28, f12, f0, f15, dyn\n"                   // opcode=0x78067e5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.ps f17, f14, f15\n"                            // opcode=0x20f708fb
		"maskpopc x0, m4\n"                                   // opcode=0x5202007b
		"fcvt.ps.pw f17, f13, dyn\n"                          // opcode=0xd006f8fb


		"fround.ps f17, f12, dyn\n"                           // opcode=0x581678fb
		"fcvt.un16.ps f17, f1\n"                              // opcode=0xd91088fb
		"feq.pi f23, f0, f12\n"                               // opcode=0xa6c02bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m1, f1, f12\n"                               // opcode=0x3ec080fb
		"flw.ps f1,  32 (x4)\n"
		"fround.ps f29, f0, dyn\n"                            // opcode=0x58107efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.s f21, f15, f15\n"                              // opcode=0x28f78ad3


		"flq2 f26, 192(x11)\n"                                // opcode=0x0d65dd07
		"fcvt.s.wu f31, x1, dyn\n"                            // opcode=0xd010ffd3
		"fbci.pi f0, 82051\n"                                 // opcode=0x2810705f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fnmsub.s f21, f12, f14, f13, dyn\n"                  // opcode=0x68e67acb
		"fcvt.un8.ps f24, f15\n"                              // opcode=0xd9378c7b




		"fle.ps f22, f12, f12\n"                              // opcode=0xa0c60b7b
		"fsat8.pi f17, f12\n"                                 // opcode=0x060638fb
		"fmul.pi f19, f1, f1\n"                               // opcode=0x161089fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pw f30, f0, dyn\n"                           // opcode=0xd0007f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"masknot m6, m3\n"                                    // opcode=0x6601a37b




		"fmsub.s f26, f0, f0, f15, dyn\n"                     // opcode=0x78007d47
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f27, f14, f13\n"                             // opcode=0x28d70dfb
		"masknot m3, m0\n"                                    // opcode=0x660021fb
		"fbcx.ps f1, x12\n"                                   // opcode=0x0006308b
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f23, f12\n"                                  // opcode=0x58860bfb
		"flt.pi f19, f15, f15\n"                              // opcode=0xa6f799fb
		"fbci.pi f27, 37771\n"                                // opcode=0x12717ddf
		"fpackrepb.pi f31, f13\n"                             // opcode=0x26068ffb


		"feq.pi f23, f12, f12\n"                              // opcode=0xa6c62bfb
		"mova.x.m x24\n"                                      // opcode=0xd6000c7b


		"flw f21, 973(x13)\n"                                 // opcode=0x3cd6aa87


		"fbc.ps f25, 128(x14)\n"                              // opcode=0x1f708c8b
		"fle.pi f30, f0, f15\n"                               // opcode=0xa6f00f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m7, f14\n"                                  // opcode=0xe00753fb
		"maskpopc x28, m0\n"                                  // opcode=0x52000e7b
		"fmv.x.s x17, f1\n"                                   // opcode=0xe00088d3
		"maskxor m7, m2, m2\n"                                // opcode=0x662143fb
		"fcvt.un10.ps f18, f0\n"                              // opcode=0xd920097b
		"fltm.ps m3, f15, f12\n"                              // opcode=0xa0c7d1fb


		"fsw f15, -943(x15)\n"                                // opcode=0xc4f7a8a7
		"fsat8.pi f17, f14\n"                                 // opcode=0x060738fb
		"fpackreph.pi f24, f15\n"                             // opcode=0x26079c7b




		"fcvt.f16.ps f0, f12\n"                               // opcode=0xd896007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f24, 120064\n"                               // opcode=0x3aa00c1f


		"fswizz.ps f23, f0, 54\n"                             // opcode=0xe6d04bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f29, f14, f1, f12, rne\n"                   // opcode=0x60170ecf
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f28, f1, f12\n"                       // opcode=0x88c09e7b
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m5, f0, f1\n"                                // opcode=0xa01052fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m1, f13\n"                                  // opcode=0xe006d0fb
		"fsgnjx.s f1, f1, f13\n"                              // opcode=0x20d0a0d3
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x26, m6\n"                                 // opcode=0x54030d7b
		"fsgnjn.s f27, f0, f1\n"                              // opcode=0x20101dd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f30, f15\n"                                  // opcode=0x58278f7b
		"fsgnj.ps f25, f12, f12\n"                            // opcode=0x20c60cfb
		"fcvt.un2.ps f1, f15\n"                               // opcode=0xd97780fb
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f29, f12, 100\n"                            // opcode=0x34860ebf
		"fand.pi f19, f1, f14\n"                              // opcode=0x06e0f9fb
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f0, f15, 12\n"                              // opcode=0x4f97d07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.s f27, f14, f15\n"                              // opcode=0x28f70dd3
		"fmv.x.s x16, f0\n"                                   // opcode=0xe0000853
		"fclass.s x24, f15\n"                                 // opcode=0xe0079c53
		"fsll.pi f23, f1, f12\n"                              // opcode=0x06c09bfb
		"flw.ps f1,  32 (x4)\n"
		"maskor m2, m2, m5\n"                                 // opcode=0x6651617b

		"li x5, 0x3113089fb2aa2373\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.sn8.ps f21, f14\n"                              // opcode=0xd9b70afb
		"fmv.s.x f24, x11\n"                                  // opcode=0xf0058c53
		"fmul.ps f0, f13, f12, dyn\n"                         // opcode=0x10c6f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f0, f0, f15, dyn\n"                           // opcode=0x08f07053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackrepb.pi f26, f13\n"                             // opcode=0x26068d7b
		"fnot.pi f0, f0\n"                                    // opcode=0x0600207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.pi f31, f12, f0\n"                               // opcode=0xa6060ffb


		"fsw f13, 975(x14)\n"                                 // opcode=0x3cd727a7


		"flw f31, -1403(x12)\n"                               // opcode=0xa850af87
		"fcvt.un10.ps f27, f15\n"                             // opcode=0xd9278dfb
		"flt.s x22, f1, f1\n"                                 // opcode=0xa0109b53
		"flw.ps f1,  32 (x4)\n"
		"fexp.ps f22, f14\n"                                  // opcode=0x58470b7b
		"flt.ps f16, f0, f14\n"                               // opcode=0xa0e0187b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f21, f15, f0\n"                             // opcode=0x1607aafb
		"fnot.pi f24, f15\n"                                  // opcode=0x0607ac7b
		"fmax.pi f29, f0, f0\n"                               // opcode=0x2e001efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsq2 f0, 864(x11)\n"                                 // opcode=0x12065da7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"maskand m4, m0, m7\n"                                // opcode=0x6670727b
		"feq.ps f17, f12, f0\n"                               // opcode=0xa00628fb
		"fltm.pi m4, f14, f1\n"                               // opcode=0x3e17027b
		"mov.m.x m0, x13, 113\n"                              // opcode=0x57c6b07b
		"fmax.ps f20, f15, f13\n"                             // opcode=0x28d79a7b
		"fmulh.pi f20, f14, f13\n"                            // opcode=0x16d72a7b
		"feq.ps f23, f14, f13\n"                              // opcode=0xa0d72bfb
		"fcvt.un10.ps f16, f12\n"                             // opcode=0xd926087b
		"mova.m.x x0\n"                                       // opcode=0xd600107b
		"masknot m1, m1\n"                                    // opcode=0x6600a0fb
		"fnmsub.s f18, f12, f14, f1, rdn\n"                   // opcode=0x08e6294b
		"flw.ps f1,  32 (x4)\n"
		"cubesgnsc.ps f30, f1, f15\n"                         // opcode=0x88f0af7b
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m5, f1, f14\n"                               // opcode=0xa0e0d2fb
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f30, f13, 52\n"                            // opcode=0xe6d68f7b


		"flq2 f19, -832(x13)\n"                               // opcode=0xabd6d987
		"fmulh.pi f31, f12, f14\n"                            // opcode=0x16e62ffb


		"fswizz.ps f27, f13, 58\n"                            // opcode=0xe6e6cdfb
		"fsgnjx.s f21, f1, f15\n"                             // opcode=0x20f0aad3
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f1, -612(x14)\n"                              // opcode=0xd8176e0b
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f19, f15, f15\n"                             // opcode=0x28f789fb


		"flw.ps f18, -827(x11)\n"                             // opcode=0xcc55a90b
		"fnmsub.s f0, f14, f1, f12, dyn\n"                    // opcode=0x6017704b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fround.ps f16, f0, dyn\n"                            // opcode=0x5810787b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f27, f14\n"                                // opcode=0x06173dfb
		"fbci.ps f21, 410280\n"                               // opcode=0xc8550a9f
		"fmv.s.x f21, x1\n"                                   // opcode=0xf0008ad3
		"fcvt.wu.s x25, f14, dyn\n"                           // opcode=0xc0177cd3
		"fsra.pi f31, f12, f0\n"                              // opcode=0x0e065ffb
		"fmaxu.pi f30, f0, f12\n"                             // opcode=0x2ec03f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f25, f13, f13\n"                             // opcode=0x20d68cd3
		"fpackreph.pi f26, f14\n"                             // opcode=0x26071d7b
		"fcvt.un16.ps f26, f13\n"                             // opcode=0xd9168d7b
		"cubesgnsc.ps f31, f0, f15\n"                         // opcode=0x88f02ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs





		"maskor m6, m3, m7\n"                                 // opcode=0x6671e37b
		"fcvt.sn8.ps f0, f15\n"                               // opcode=0xd9b7807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fmv.x.s x21, f1\n"                                   // opcode=0xe0008ad3
		"fsgnjn.ps f24, f12, f0\n"                            // opcode=0x20061c7b
		"fnmsub.ps f31, f12, f15, f0, dyn\n"                  // opcode=0x04f67fdb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgnsc.ps f30, f0, f0\n"                          // opcode=0x88002f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fminu.pi f24, f1, f14\n"                             // opcode=0x2ee0ac7b
		"flw.ps f1,  32 (x4)\n"
		"fsra.pi f24, f15, f13\n"                             // opcode=0x0ed7dc7b
		"frcp.ps f23, f15\n"                                  // opcode=0x58778bfb


		"fmsub.ps f24, f14, f1, f1, dyn\n"                    // opcode=0x0a177c5b
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f22, f15, 8\n"                              // opcode=0x4f179b7b
		"fle.pi f1, f12, f14\n"                               // opcode=0xa6e600fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x29, f0, dyn\n"                             // opcode=0xc0007ed3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f17, 321830\n"                               // opcode=0x9d24c8df
		"fsra.pi f1, f15, f13\n"                              // opcode=0x0ed7d0fb
		"flw.ps f1,  32 (x4)\n"
		"fltm.pi m5, f12, f12\n"                              // opcode=0x3ec602fb
		"fslli.pi f30, f1, 5\n"                               // opcode=0x4eb09f7b
		"flw.ps f1,  32 (x4)\n"
		"flem.ps m2, f12, f12\n"                              // opcode=0xa0c6417b
		"fmsub.s f1, f0, f0, f12, dyn\n"                      // opcode=0x600070c7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fnot.pi f31, f13\n"                                  // opcode=0x0606affb
		"flt.pi f17, f0, f12\n"                               // opcode=0xa6c018fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un8.ps f31, f13\n"                              // opcode=0xd9368ffb
		"fle.ps f1, f14, f14\n"                               // opcode=0xa0e700fb
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f27, f15, 11\n"                             // opcode=0x4f77ddfb
		"fcvt.s.w f28, x11, dyn\n"                            // opcode=0xd005fe53
		"fmulh.pi f29, f12, f14\n"                            // opcode=0x16e62efb
		"fmul.ps f0, f15, f14, dyn\n"                         // opcode=0x10e7f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmov.ps f18, f13, f13, f15\n"                       // opcode=0x7cd6a93f
		"fmadd.s f28, f15, f0, f0, dyn\n"                     // opcode=0x0007fe43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"flem.ps m1, f13, f13\n"                              // opcode=0xa0d6c0fb
		"frcp.ps f20, f0\n"                                   // opcode=0x58700a7b
		"fclass.ps f22, f13\n"                                // opcode=0xe0069b7b
		"fand.pi f24, f1, f13\n"                              // opcode=0x06d0fc7b
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f30, f13, f14\n"                              // opcode=0xa0e6af7b
		"fadd.s f22, f12, f14, dyn\n"                         // opcode=0x00e67b53
		"fcvt.w.s x0, f0, dyn\n"                              // opcode=0xc0007053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsll.pi f31, f15, f15\n"                             // opcode=0x06f79ffb
		"fsatu8.pi f16, f1\n"                                 // opcode=0x0610b87b
		"feq.pi f19, f1, f1\n"                                // opcode=0xa610a9fb
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f26, f15, f13\n"                             // opcode=0x28d78d7b
		"fmul.s f29, f1, f14, dyn\n"                          // opcode=0x10e0fed3
		"flw.ps f1,  32 (x4)\n"




		"fle.pi f16, f13, f0\n"                               // opcode=0xa606887b


		"fsq2 f15, -1696(x15)\n"                              // opcode=0xc8f0d427
		"fadd.ps f19, f0, f0, dyn\n"                          // opcode=0x000079fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f26, f0, f0\n"                               // opcode=0x28000d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m1, m7, m1\n"                                 // opcode=0x6613e0fb
		//RTLMIN-5479"frsq.ps f30, f14\n"                                  // opcode=0x58870f7b
		"fpackrepb.pi f30, f14\n"                             // opcode=0x26070f7b
		"fle.ps f28, f13, f14\n"                              // opcode=0xa0e68e7b

		"li x5, 0x428c67d2d5d75ef3\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcmov.ps f17, f12, f1, f14\n"                        // opcode=0x741628bf
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f25, f0, f14\n"                            // opcode=0x00e00cf7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.ps f20, f13, f14\n"                             // opcode=0x28e69a7b
		"fcvt.f16.ps f26, f14\n"                              // opcode=0xd8970d7b
		"maskand m7, m0, m3\n"                                // opcode=0x663073fb
		"fcvt.s.w f27, x12, dyn\n"                            // opcode=0xd0067dd3
		"fltm.ps m0, f1, f14\n"                               // opcode=0xa0e0d07b
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m1, x0, 96\n"                                // opcode=0x578000fb
		"flt.s x22, f15, f12\n"                               // opcode=0xa0c79b53
		"feqm.ps m0, f14, f15\n"                              // opcode=0xa0f7607b
		"fmvs.x.ps x17, f0, 1\n"                              // opcode=0xe01028fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.sn8.ps f24, f15\n"                              // opcode=0xd9b78c7b
		"fsatu8.pi f27, f13\n"                                // opcode=0x0616bdfb
		"fclass.s x25, f0\n"                                  // opcode=0xe0001cd3
		"fpackrepb.pi f19, f14\n"                             // opcode=0x260709fb
		"fsgnjx.s f25, f13, f1\n"                             // opcode=0x2016acd3
		"fcvt.pw.ps f28, f1, dyn\n"                           // opcode=0xc000fe7b
		"flw.ps f1,  32 (x4)\n"
		"masknot m2, m4\n"                                    // opcode=0x6602217b
		"fmin.pi f25, f0, f15\n"                              // opcode=0x2ef00cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.ps f24, f0, f1\n"                                // opcode=0xa0101c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f18, f13, f13\n"                            // opcode=0x16d6a97b
		"fmax.pi f21, f15, f13\n"                             // opcode=0x2ed79afb
		"fmul.pi f27, f15, f15\n"                             // opcode=0x16f78dfb
		"maskpopcz x23, m2\n"                                 // opcode=0x54010bfb
		"frcp.ps f17, f14\n"                                  // opcode=0x587708fb
		"feq.pi f21, f15, f15\n"                              // opcode=0xa6f7aafb
		"fadd.pi f23, f15, f12\n"                             // opcode=0x06c78bfb
		"fle.s x1, f13, f14\n"                                // opcode=0xa0e680d3


		"fswizz.ps f1, f15, 79\n"                             // opcode=0xe737e0fb
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f29, f15, f12\n"                             // opcode=0x06c7cefb
		"fmin.ps f24, f14, f12\n"                             // opcode=0x28c70c7b
		"frcp.ps f27, f1\n"                                   // opcode=0x58708dfb
		"fadd.ps f22, f13, f12, dyn\n"                        // opcode=0x00c6fb7b
		"fmvs.x.ps x27, f12, 7\n"                             // opcode=0xe0762dfb
		"fmadd.s f22, f13, f13, f0, dyn\n"                    // opcode=0x00d6fb43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f0, f1, f15\n"                               // opcode=0x0ef0807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fand.pi f22, f13, f15\n"                             // opcode=0x06f6fb7b
		"fminu.pi f0, f15, f13\n"                             // opcode=0x2ed7a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"masknot m0, m4\n"                                    // opcode=0x6602207b
		//RTLMIN-5479"frsq.ps f24, f15\n"                                  // opcode=0x58878c7b
		"fpackreph.pi f24, f0\n"                              // opcode=0x26001c7b
		"fxor.pi f24, f0, f1\n"                               // opcode=0x06104c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f22, -32(x13)\n"                              // opcode=0xca768b0b
		"fcvt.un16.ps f31, f1\n"                              // opcode=0xd9108ffb


		"fbc.ps f29, -96(x15)\n"                              // opcode=0xfc678e8b
		"mova.x.m x0\n"                                       // opcode=0xd600007b
		"fcvt.s.w f31, x14, dyn\n"                            // opcode=0xd0077fd3
		"fnot.pi f27, f15\n"                                  // opcode=0x0607adfb
		"fclass.s x24, f1\n"                                  // opcode=0xe0009c53
		"fpackrepb.pi f24, f12\n"                             // opcode=0x26060c7b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.ps f24, f12, f0\n"                               // opcode=0xa0061c7b
		"fsgnjn.s f30, f12, f13\n"                            // opcode=0x20d61f53
		"fsub.s f16, f1, f14, dyn\n"                          // opcode=0x08e0f853
		"flw.ps f1,  32 (x4)\n"


		"mov.m.x m3, x13, 107\n"                              // opcode=0x57a6e1fb
		"fmadd.ps f18, f0, f14, f15, dyn\n"                   // opcode=0x78e0795b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f23, f1\n"                               // opcode=0xd8908bfb
		"fpackreph.pi f30, f0\n"                              // opcode=0x26001f7b
		"fclass.ps f27, f13\n"                                // opcode=0xe0069dfb
		"fslli.pi f19, f14, 8\n"                              // opcode=0x4f0719fb
		"fcvt.ps.pwu f23, f0, dyn\n"                          // opcode=0xd0107bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f21, f1, f0\n"                              // opcode=0x20009ad3
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f0, f12\n"                                   // opcode=0x5886007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.ps f30, f0, f14\n"                              // opcode=0x28e01f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f1, f15, dyn\n"                           // opcode=0xd007f0fb
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f30, f13, f13\n"                            // opcode=0x2ed6bf7b
		"fsub.pi f30, f13, f13\n"                             // opcode=0x0ed68f7b
		"fmulhu.pi f28, f13, f15\n"                           // opcode=0x16f69e7b
		"fcvt.f16.ps f31, f13\n"                              // opcode=0xd8968ffb




		"mova.x.m x16\n"                                      // opcode=0xd600087b
		"fmaxu.pi f0, f13, f0\n"                              // opcode=0x2e06b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x15\n"                                      // opcode=0xd607907b
		"fclass.ps f1, f15\n"                                 // opcode=0xe00790fb
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f22, -192(x11)\n"                             // opcode=0x9ce58b0b


		"flq2 f31, 640(x11)\n"                                // opcode=0x75075f87
		"fpackrepb.pi f28, f0\n"                              // opcode=0x26000e7b
		"fmax.pi f18, f14, f13\n"                             // opcode=0x2ed7197b
		"frcp.ps f24, f0\n"                                   // opcode=0x58700c7b
		"fandi.pi f19, f0, 85\n"                              // opcode=0x2cb019bf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f19, f15\n"                              // opcode=0xd89789fb




		"fsub.s f19, f15, f15, rmm\n"                         // opcode=0x08f7c9d3
		"fmv.x.s x21, f13\n"                                  // opcode=0xe0068ad3
		"maskor m4, m6, m4\n"                                 // opcode=0x6643627b




		"fsat8.pi f1, f1\n"                                   // opcode=0x0600b0fb
		"flw.ps f1,  32 (x4)\n"
		"fmul.s f19, f13, f1, dyn\n"                          // opcode=0x1016f9d3
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f0, f15, 211\n"                             // opcode=0x6c67803f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fltu.pi f17, f15, f0\n"                              // opcode=0xa607b8fb
		"fcvt.un2.ps f30, f14\n"                              // opcode=0xd9770f7b
		"fcvt.f16.ps f23, f13\n"                              // opcode=0xd8968bfb
		"fmax.ps f25, f14, f15\n"                             // opcode=0x28f71cfb
		"for.pi f23, f0, f13\n"                               // opcode=0x06d06bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f0, x12, dyn\n"                             // opcode=0xd0067053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.ps f0, f14, f15\n"                               // opcode=0xa0f7107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f18, -1300(x12)\n"                               // opcode=0xaec62907
		"fcvt.un16.ps f22, f13\n"                             // opcode=0xd9168b7b
		"fmin.pi f19, f13, f12\n"                             // opcode=0x2ec689fb
		//RTLMIN-5479"fsin.ps f30, f12\n"                                  // opcode=0x58660f7b
		"fsgnjx.s f21, f15, f0\n"                             // opcode=0x2007aad3
		"flt.pi f23, f13, f0\n"                               // opcode=0xa6069bfb
		"fcvt.pwu.ps f26, f0, rmm\n"                          // opcode=0xc0104d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.wu f31, x15, dyn\n"                           // opcode=0xd017ffd3

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmv.s.x f17, x0\n"                                   // opcode=0xf00008d3
		"maskor m2, m7, m1\n"                                 // opcode=0x6613e17b
		"fcvt.pw.ps f22, f0, dyn\n"                           // opcode=0xc0007b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"masknot m3, m3\n"                                    // opcode=0x6601a1fb
		"fmadd.ps f29, f15, f13, f14, dyn\n"                  // opcode=0x70d7fedb
		//RTLMIN-5479"fsin.ps f0, f13\n"                                   // opcode=0x5866807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f28, f1, f13\n"                              // opcode=0xa6d0be7b
		"flw.ps f1,  32 (x4)\n"
		"fmul.ps f25, f14, f12, dyn\n"                        // opcode=0x10c77cfb
		"fnot.pi f24, f13\n"                                  // opcode=0x0606ac7b
		"fmax.s f1, f0, f12\n"                                // opcode=0x28c010d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pw f22, f1, dyn\n"                           // opcode=0xd000fb7b
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f1, f15, f0\n"                        // opcode=0x880790fb
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f23, f1\n"                                  // opcode=0x0600bbfb
		"masknot m3, m6\n"                                    // opcode=0x660321fb
		"fround.ps f18, f14, rtz\n"                           // opcode=0x5817197b
		"fbcx.ps f25, x13\n"                                  // opcode=0x0006bc8b
		"maskxor m0, m4, m1\n"                                // opcode=0x6612407b
		"fbci.pi f1, 45551\n"                                 // opcode=0x163de0df
		"flw.ps f1,  32 (x4)\n"


		"flq2 f1, -1280(x11)\n"                               // opcode=0xd695d087
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f27, f12\n"                                // opcode=0xe0061dfb
		"fcvt.s.w f23, x14, dyn\n"                            // opcode=0xd0077bd3
		"fsll.pi f23, f13, f12\n"                             // opcode=0x06c69bfb




		"frcp.ps f24, f0\n"                                   // opcode=0x58700c7b
		"masknot m2, m4\n"                                    // opcode=0x6602217b
		"fmvs.x.ps x26, f12, 5\n"                             // opcode=0xe0562d7b
		"fadd.ps f27, f14, f12, dyn\n"                        // opcode=0x00c77dfb
		"fslli.pi f0, f14, 13\n"                              // opcode=0x4fb7107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flem.ps m2, f13, f1\n"                               // opcode=0xa016c17b
		"fminu.pi f28, f13, f0\n"                             // opcode=0x2e06ae7b
		"faddi.pi f0, f12, 257\n"                             // opcode=0x8426003f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fandi.pi f1, f12, 49\n"                              // opcode=0x1c3610bf
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.f16 f1, f1\n"                                // opcode=0xd0a080fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x31, f13, dyn\n"                            // opcode=0xc006ffd3
		"fsll.pi f31, f0, f12\n"                              // opcode=0x06c01ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f31, f1, f13\n"                             // opcode=0x20d09fd3
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f18, f14, f1, f15, dyn\n"                   // opcode=0x7817795b
		"flw.ps f1,  32 (x4)\n"
		"feq.pi f30, f0, f1\n"                                // opcode=0xa6102f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un8.ps f19, f15\n"                              // opcode=0xd93789fb
		"fmin.s f25, f0, f15\n"                               // opcode=0x28f00cd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f30, f15, f15\n"                         // opcode=0x88f78f7b
		"fcvt.un8.ps f31, f1\n"                               // opcode=0xd9308ffb
		"faddi.pi f29, f13, 370\n"                            // opcode=0xbc568ebf
		"cubesgntc.ps f16, f15, f1\n"                         // opcode=0x8817b87b
		"maskand m4, m0, m4\n"                                // opcode=0x6640727b
		"fcvt.ps.f16 f16, f12\n"                              // opcode=0xd0a6087b
		"fsrai.pi f22, f1, 3\n"                               // opcode=0x4e70fb7b
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x29, f1\n"                                  // opcode=0xe0009ed3
		"fcvt.s.w f27, x13, dyn\n"                            // opcode=0xd006fdd3
		"fle.s x26, f13, f13\n"                               // opcode=0xa0d68d53
		"fle.pi f17, f0, f13\n"                               // opcode=0xa6d008fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskxor m6, m6, m1\n"                                // opcode=0x6613437b
		"fnmadd.s f1, f14, f15, f15, dyn\n"                   // opcode=0x78f770cf
		"flw.ps f1,  32 (x4)\n"
		"fpackreph.pi f18, f12\n"                             // opcode=0x2606197b
		"fpackrepb.pi f27, f1\n"                              // opcode=0x26008dfb
		"fmsub.s f24, f1, f0, f15, dyn\n"                     // opcode=0x7800fc47
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubesgntc.ps f24, f14, f1\n"                         // opcode=0x88173c7b
		"fsgnjn.s f26, f0, f1\n"                              // opcode=0x20101d53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fminu.pi f21, f14, f14\n"                            // opcode=0x2ee72afb
		"flt.ps f20, f13, f13\n"                              // opcode=0xa0d69a7b
		"fbcx.ps f0, x0\n"                                    // opcode=0x0000300b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fsub.pi f26, f15, f14\n"                             // opcode=0x0ee78d7b
		"fsat8.pi f20, f12\n"                                 // opcode=0x06063a7b




		"flt.s x21, f15, f14\n"                               // opcode=0xa0e79ad3


		"fsw.ps f0, -353(x13)\n"                              // opcode=0xe806ef8b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f19, f14, f15\n"                             // opcode=0xa6f739fb


		"flq2 f27, 1024(x11)\n"                               // opcode=0x23b5dd87
		"fmulhu.pi f1, f12, f0\n"                             // opcode=0x160610fb
		"flw.ps f1,  32 (x4)\n"
		"feq.s x22, f1, f15\n"                                // opcode=0xa0f0ab53
		"flw.ps f1,  32 (x4)\n"
		"fmax.s f0, f1, f12\n"                                // opcode=0x28c09053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f23, f1, f14\n"                               // opcode=0xa6e08bfb
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x19, m4\n"                                  // opcode=0x520209fb


		"fsw.ps f13, -1534(x11)\n"                            // opcode=0xa0d5e10b
		"fsub.s f17, f1, f15, dyn\n"                          // opcode=0x08f0f8d3
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f27, x13, dyn\n"                            // opcode=0xd006fdd3


		"flq2 f1, -1536(x12)\n"                               // opcode=0x97f65087
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f30, f1, f14\n"                             // opcode=0x20e0af53
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f20, f1, f14\n"                               // opcode=0xa0e09a7b
		"flw.ps f1,  32 (x4)\n"
		"fsrai.pi f0, f1, 15\n"                               // opcode=0x4fe0f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f22, f1, f0\n"                                // opcode=0x28008b53
		"flw.ps f1,  32 (x4)\n"
		"fmax.s f23, f12, f12\n"                              // opcode=0x28c61bd3
		"fmul.pi f26, f15, f14\n"                             // opcode=0x16e78d7b
		"faddi.pi f16, f12, 279\n"                            // opcode=0x8cf6083f
		"fcvt.un16.ps f1, f13\n"                              // opcode=0xd91680fb
		"flw.ps f1,  32 (x4)\n"
		"fmin.pi f28, f14, f15\n"                             // opcode=0x2ef70e7b
		"fround.ps f17, f13, dyn\n"                           // opcode=0x5816f8fb
		"fadd.pi f18, f13, f1\n"                              // opcode=0x0616897b
		"fround.ps f22, f14, dyn\n"                           // opcode=0x58177b7b


		"flw f31, 1694(x12)\n"                                // opcode=0x69e5af87
		"fminu.pi f28, f15, f0\n"                             // opcode=0x2e07ae7b
		"cubefaceidx.ps f17, f15, f1\n"                       // opcode=0x881798fb




		"fcvt.pwu.ps f21, f1, dyn\n"                          // opcode=0xc010fafb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f25, f0, f14\n"                            // opcode=0x20e02cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fslli.pi f0, f15, 14\n"                              // opcode=0x4fd7907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.w.s x17, f12, dyn\n"                            // opcode=0xc00678d3
		"cubesgntc.ps f1, f0, f15\n"                          // opcode=0x88f030fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.s f27, f13, f14, f12, dyn\n"                  // opcode=0x60e6fdcf
		"fcvt.un8.ps f19, f1\n"                               // opcode=0xd93089fb
		"fbci.ps f19, 431380\n"                               // opcode=0xd2a2999f
		"fmv.x.s x18, f14\n"                                  // opcode=0xe0070953
		"fcvt.sn16.ps f28, f1\n"                              // opcode=0xd9908e7b

		"li x5, 0x35e216d047109700\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbcx.ps f0, x15\n"                                   // opcode=0x0007b00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"frcp.ps f1, f0\n"                                    // opcode=0x587000fb
		"flw.ps f1,  32 (x4)\n"
		"fpackreph.pi f30, f14\n"                             // opcode=0x26071f7b




		"fsgnjx.s f29, f15, f13\n"                            // opcode=0x20d7aed3
		"fbci.ps f26, 146208\n"                               // opcode=0x47640d1f
		"masknot m6, m5\n"                                    // opcode=0x6602a37b
		//RTLMIN-5479"frsq.ps f30, f1\n"                                   // opcode=0x58808f7b
		"maskpopc x30, m7\n"                                  // opcode=0x52038f7b
		"fpackrepb.pi f24, f12\n"                             // opcode=0x26060c7b
		"maskor m3, m1, m1\n"                                 // opcode=0x6610e1fb
		"fmvs.x.ps x20, f13, 6\n"                             // opcode=0xe066aa7b
		"fsgnjn.ps f22, f0, f0\n"                             // opcode=0x20001b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f30, f14, 11\n"                             // opcode=0x4f775f7b
		"fsgnjx.ps f26, f13, f1\n"                            // opcode=0x2016ad7b
		"mov.m.x m4, x14, 11\n"                               // opcode=0x5627627b
		"fsgnjx.s f31, f1, f0\n"                              // opcode=0x2000afd3
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f27, f13, 5\n"                              // opcode=0x4ea69dfb
		"fsgnjn.s f24, f1, f15\n"                             // opcode=0x20f09c53
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f20, f15, f15, f13, dyn\n"                  // opcode=0x68f7fa5b


		"fsw f15, 1848(x12)\n"                                // opcode=0x72f5ac27


		"flw f20, -229(x13)\n"                                // opcode=0xf1b6aa07
		"fexp.ps f17, f12\n"                                  // opcode=0x584608fb
		"maskpopcz x29, m6\n"                                 // opcode=0x54030efb
		//RTLMIN-5479"fsin.ps f22, f12\n"                                  // opcode=0x58660b7b
		"fcvt.sn8.ps f20, f12\n"                              // opcode=0xd9b60a7b
		"flt.s x29, f14, f13\n"                               // opcode=0xa0d71ed3
		"fcvt.s.wu f24, x13, dyn\n"                           // opcode=0xd016fc53
		"fsub.pi f25, f14, f15\n"                             // opcode=0x0ef70cfb
		"fsrli.pi f26, f0, 3\n"                               // opcode=0x4e705d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"for.pi f19, f0, f14\n"                               // opcode=0x06e069fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f1, f13, f0\n"                               // opcode=0x2e0680fb
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f1, 160(x12)\n"                               // opcode=0x5790008b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f24, f15, dyn\n"                          // opcode=0xc007fc7b
		"fsgnjn.s f18, f0, f15\n"                             // opcode=0x20f01953
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m3, x0, 69\n"                                // opcode=0x571021fb
		"fsat8.pi f21, f13\n"                                 // opcode=0x0606bafb




		"maskand m2, m5, m5\n"                                // opcode=0x6652f17b
		"fbcx.ps f16, x13\n"                                  // opcode=0x0006b80b
		"fpackreph.pi f20, f15\n"                             // opcode=0x26079a7b
		"fcvt.w.s x23, f14, dyn\n"                            // opcode=0xc0077bd3


		"flw.ps f0, 873(x15)\n"                               // opcode=0x3697200b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f21, f15, f12, dyn\n"                         // opcode=0x08c7fad3
		"cubefaceidx.ps f31, f15, f12\n"                      // opcode=0x88c79ffb
		//RTLMIN-5479"frsq.ps f0, f14\n"                                   // opcode=0x5887007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f30, f1\n"                                   // opcode=0x58708f7b


		"fbc.ps f27, -1120(x13)\n"                            // opcode=0xb4068d8b
		"fadd.s f31, f13, f1, dyn\n"                          // opcode=0x0016ffd3
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f30, f0, f15, f15, dyn\n"                   // opcode=0x78f07f5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m0, f15, f14\n"                              // opcode=0xa0e7e07b
		"fcvt.s.w f31, x14, dyn\n"                            // opcode=0xd0077fd3

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flog.ps f21, f14\n"                                  // opcode=0x58370afb
		"fpackreph.pi f19, f15\n"                             // opcode=0x260799fb
		"fcvt.w.s x29, f15, rdn\n"                            // opcode=0xc007aed3
		"cubesgntc.ps f21, f15, f0\n"                         // opcode=0x8807bafb
		"ffrc.ps f26, f15\n"                                  // opcode=0x58278d7b
		"fmul.s f25, f13, f12, dyn\n"                         // opcode=0x10c6fcd3
		"ffrc.ps f17, f13\n"                                  // opcode=0x582688fb
		"fmaxu.pi f22, f0, f14\n"                             // opcode=0x2ee03b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.ps f20, f12, f14, dyn\n"                        // opcode=0x08e67a7b
		"fcvt.sn16.ps f19, f13\n"                             // opcode=0xd99689fb
		"fbcx.ps f31, x12\n"                                  // opcode=0x00063f8b
		"fnot.pi f25, f0\n"                                   // opcode=0x06002cfb
		"fnmsub.ps f31, f14, f13, f13, dyn\n"                 // opcode=0x6cd77fdb
		"fsrl.pi f1, f1, f13\n"                               // opcode=0x06d0d0fb
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f17, f12\n"                                // opcode=0xe00618fb
		"fandi.pi f22, f1, 453\n"                             // opcode=0xe4a09b3f
		"flw.ps f1,  32 (x4)\n"


		"fcvt.s.w f25, x13, dyn\n"                            // opcode=0xd006fcd3


		"fsw.ps f1, 1213(x15)\n"                              // opcode=0x4a106e8b
		"flw.ps f1,  32 (x4)\n"


		"fcvt.w.s x1, f12, dyn\n"                             // opcode=0xc00670d3


		"fswizz.ps f28, f0, 51\n"                             // opcode=0xe6c06e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fmul.s f19, f13, f12, dyn\n"                         // opcode=0x10c6f9d3
		"for.pi f23, f15, f1\n"                               // opcode=0x0617ebfb
		"fcvt.pwu.ps f31, f0, dyn\n"                          // opcode=0xc0107ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f26, 81340\n"                                // opcode=0x27b79d1f
		"fnmadd.s f0, f14, f13, f1, dyn\n"                    // opcode=0x08d7704f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fcvt.s.wu f16, x11, rup\n"                           // opcode=0xd015b853
		"fcvt.ps.pw f18, f14, dyn\n"                          // opcode=0xd007797b
		"fmadd.ps f23, f1, f12, f1, dyn\n"                    // opcode=0x08c0fbdb
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f20, f12, 137\n"                            // opcode=0x45361a3f
		"mov.m.x m0, x14, 78\n"                               // opcode=0x5737407b
		"masknot m4, m4\n"                                    // opcode=0x6602227b
		"fbci.ps f17, 389092\n"                               // opcode=0xbdfc889f
		"maskpopc x19, m4\n"                                  // opcode=0x520209fb
		"fsrli.pi f17, f13, 12\n"                             // opcode=0x4f96d8fb
		"maskand m4, m2, m5\n"                                // opcode=0x6651727b
		"fmin.pi f31, f14, f14\n"                             // opcode=0x2ee70ffb
		"fbci.ps f25, 283579\n"                               // opcode=0x8a777c9f
		"fsgnjn.ps f29, f1, f12\n"                            // opcode=0x20c09efb
		"flw.ps f1,  32 (x4)\n"
		"maskand m6, m7, m5\n"                                // opcode=0x6653f37b
		"flog.ps f31, f14\n"                                  // opcode=0x58370ffb
		"fmin.pi f18, f1, f1\n"                               // opcode=0x2e10897b
		"flw.ps f1,  32 (x4)\n"


		"flw f0, 1966(x11)\n"                                 // opcode=0x7ae62007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvs.x.ps x30, f13, 2\n"                             // opcode=0xe026af7b






		"fsw f15, 1793(x15)\n"                                // opcode=0x70f5a0a7
		"fmin.pi f31, f15, f1\n"                              // opcode=0x2e178ffb




		"fcvt.ps.pw f28, f15, dyn\n"                          // opcode=0xd007fe7b
		"fnot.pi f23, f1\n"                                   // opcode=0x0600abfb
		"fsgnj.ps f29, f14, f0\n"                             // opcode=0x20070efb
		"fmin.pi f16, f1, f14\n"                              // opcode=0x2ee0887b
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0x4c931aa560cbdba9\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmadd.s f22, f14, f1, f15, dyn\n"                    // opcode=0x78177b43
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f16, f14, f1\n"                               // opcode=0xa617187b
		"fsra.pi f27, f12, f14\n"                             // opcode=0x0ee65dfb
		"fmv.x.s x26, f13\n"                                  // opcode=0xe0068d53
		"fltu.pi f29, f1, f0\n"                               // opcode=0xa600befb
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f23, f15, 26\n"                             // opcode=0x0d479bbf


		"flw f26, -830(x11)\n"                                // opcode=0xcc25ad07
		"fandi.pi f24, f14, 434\n"                            // opcode=0xdc571c3f
		"fmul.pi f28, f13, f15\n"                             // opcode=0x16f68e7b
		"fcmov.ps f31, f14, f1, f15\n"                        // opcode=0x7c172fbf
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f29, f13\n"                              // opcode=0xd8968efb


		"fbc.ps f0, 1952(x12)\n"                              // opcode=0x50d6800b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.sn16.ps f26, f14\n"                             // opcode=0xd9970d7b
		"fexp.ps f16, f0\n"                                   // opcode=0x5840087b
		"maskpopcz x1, m1\n"                                  // opcode=0x540080fb
		"fcvt.s.w f26, x14, rtz\n"                            // opcode=0xd0071d53
		"fexp.ps f21, f0\n"                                   // opcode=0x58400afb
		"maskand m7, m7, m0\n"                                // opcode=0x6603f3fb
		"fsub.ps f16, f13, f13, dyn\n"                        // opcode=0x08d6f87b
		"fbci.ps f31, 86821\n"                                // opcode=0x2a64bf9f
		"fmul.ps f18, f15, f13, dyn\n"                        // opcode=0x10d7f97b
		"feq.s x25, f0, f15\n"                                // opcode=0xa0f02cd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un8.ps f26, f13\n"                              // opcode=0xd9368d7b
		"fmax.s f21, f1, f13\n"                               // opcode=0x28d09ad3
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un16.ps f23, f0\n"                              // opcode=0xd9100bfb
		"fmul.ps f19, f15, f13, dyn\n"                        // opcode=0x10d7f9fb
		"feq.pi f30, f13, f1\n"                               // opcode=0xa616af7b
		"fcvt.un24.ps f19, f0\n"                              // opcode=0xd90009fb


		"fswizz.ps f19, f15, 77\n"                            // opcode=0xe737a9fb
		"fsgnjx.ps f18, f1, f1\n"                             // opcode=0x2010a97b
		"flw.ps f1,  32 (x4)\n"




		"feq.ps f23, f15, f1\n"                               // opcode=0xa017abfb


		"flw.ps f16, -742(x15)\n"                             // opcode=0xd1a7a80b
		"fbcx.ps f0, x13\n"                                   // opcode=0x0006b00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pw.ps f24, f1, dyn\n"                           // opcode=0xc000fc7b
		"flw.ps f1,  32 (x4)\n"


		"flq2 f20, 1120(x12)\n"                               // opcode=0x57c05a07
		"fcvt.un16.ps f26, f15\n"                             // opcode=0xd9178d7b
		"fle.s x30, f14, f0\n"                                // opcode=0xa0070f53


		"fswizz.ps f24, f0, 60\n"                             // opcode=0xe6f00c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f27, f0, f15, dyn\n"                          // opcode=0x10f07dd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.s f0, f1, f14\n"                                // opcode=0x28e09053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pw f16, f14, dyn\n"                          // opcode=0xd007787b
		"fnmadd.s f30, f13, f12, f15, dyn\n"                  // opcode=0x78c6ff4f
		"fnmadd.s f25, f12, f15, f13, dyn\n"                  // opcode=0x68f67ccf
		"flog.ps f31, f13\n"                                  // opcode=0x58368ffb
		"fxor.pi f23, f15, f12\n"                             // opcode=0x06c7cbfb
		"fcvt.ps.pwu f22, f15, dyn\n"                         // opcode=0xd017fb7b


		"flw f17, 1256(x13)\n"                                // opcode=0x4e802887
		"fmax.s f17, f12, f12\n"                              // opcode=0x28c618d3


		"flw.ps f22, 439(x13)\n"                              // opcode=0x1b76ab0b




		"fbci.ps f26, 336615\n"                               // opcode=0xa45ced1f

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.s x31, f1, f12\n"                                // opcode=0xa0c09fd3
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f16, f14, f1\n"                               // opcode=0xa017287b
		"fcvt.un2.ps f16, f14\n"                              // opcode=0xd977087b
		"maskand m2, m2, m6\n"                                // opcode=0x6661717b
		"fmsub.s f24, f13, f13, f15, rne\n"                   // opcode=0x78d68c47
		"fcvt.s.w f0, x14, dyn\n"                             // opcode=0xd0077053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fround.ps f25, f15, rup\n"                           // opcode=0x5817bcfb
		"fmax.pi f31, f14, f0\n"                              // opcode=0x2e071ffb
		"feq.s x24, f0, f14\n"                                // opcode=0xa0e02c53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.wu f31, x1, dyn\n"                            // opcode=0xd010ffd3
		"fle.s x1, f15, f14\n"                                // opcode=0xa0e780d3


		"fswizz.ps f20, f0, 81\n"                             // opcode=0xe7402a7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.pi f27, f0, f13\n"                              // opcode=0x16d00dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f17, f13, f0\n"                              // opcode=0xa606b8fb
		"fadd.s f25, f14, f12, dyn\n"                         // opcode=0x00c77cd3
		"maskxor m6, m0, m1\n"                                // opcode=0x6610437b
		"fcvt.un24.ps f17, f13\n"                             // opcode=0xd90688fb


		"fle.s x17, f15, f1\n"                                // opcode=0xa01788d3
		"fcvt.wu.s x21, f0, dyn\n"                            // opcode=0xc0107ad3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f17, f14, f12\n"                              // opcode=0xa0c728fb


		"flw f16, -1469(x13)\n"                               // opcode=0xa4302807


		"fsq2 f0, 32(x14)\n"                                  // opcode=0x2406daa7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskand m4, m7, m1\n"                                // opcode=0x6613f27b
		"fcvt.s.w f17, x0, dyn\n"                             // opcode=0xd00078d3
		//RTLMIN-5479"frsq.ps f30, f15\n"                                  // opcode=0x58878f7b
		"maskpopcz x18, m4\n"                                 // opcode=0x5402097b
		"fadd.s f17, f13, f1, dyn\n"                          // opcode=0x0016f8d3
		"flw.ps f1,  32 (x4)\n"
		"fadd.s f1, f12, f0, rne\n"                           // opcode=0x000600d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f29, f14, f13\n"                             // opcode=0x16d70efb
		"fmax.pi f31, f1, f13\n"                              // opcode=0x2ed09ffb
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f0, f1, f14\n"                                // opcode=0xa0e0907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.ps f19, f12, f12, f1, dyn\n"                  // opcode=0x0ec679db
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f28, f0, f0\n"                                // opcode=0xa0000e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.ps f23, f14, f12\n"                              // opcode=0xa0c70bfb
		"fround.ps f26, f1, dyn\n"                            // opcode=0x5810fd7b
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f1, f14, f13\n"                            // opcode=0x20d710fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un8.ps f19, f12\n"                              // opcode=0xd93609fb
		"cubesgnsc.ps f20, f0, f1\n"                          // opcode=0x88102a7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f20, 384(x11)\n"                                 // opcode=0x1807aa07
		"fsrli.pi f23, f14, 7\n"                              // opcode=0x4ee75bfb
		"faddi.pi f26, f15, 383\n"                            // opcode=0xbdf78d3f
		"cubesgnsc.ps f24, f15, f14\n"                        // opcode=0x88e7ac7b
		"fmax.pi f17, f15, f0\n"                              // opcode=0x2e0798fb
		"fle.pi f16, f14, f1\n"                               // opcode=0xa617087b
		"fsgnjn.s f21, f12, f12\n"                            // opcode=0x20c61ad3


		"fbc.ps f29, -1056(x12)\n"                            // opcode=0x98e60e8b
		"masknot m6, m6\n"                                    // opcode=0x6603237b
		"fsgnjx.ps f0, f12, f0\n"                             // opcode=0x2006207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f22, f12, 443\n"                            // opcode=0xdd760b3f
		"fmax.s f16, f0, f12\n"                               // opcode=0x28c01853
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsgnj.ps f27, f14, f12\n"                            // opcode=0x20c70dfb
		"fsrli.pi f1, f12, 7\n"                               // opcode=0x4ee650fb
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f31, f12, f14\n"                             // opcode=0x28e60ffb
		"masknot m5, m1\n"                                    // opcode=0x6600a2fb
		"fsetm.pi m2, f14\n"                                  // opcode=0xe007517b
		"fsll.pi f19, f15, f1\n"                              // opcode=0x061799fb
		"fle.pi f25, f12, f12\n"                              // opcode=0xa6c60cfb
		"maskor m3, m1, m3\n"                                 // opcode=0x6630e1fb
		"mov.m.x m7, x15, 40\n"                               // opcode=0x56a793fb
		"flt.pi f24, f12, f13\n"                              // opcode=0xa6d61c7b
		"fcvt.w.s x19, f13, rdn\n"                            // opcode=0xc006a9d3
		"fsrli.pi f1, f0, 3\n"                                // opcode=0x4e6050fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f16, f0, f12, rmm\n"                         // opcode=0x00c0487b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f17, f14\n"                                // opcode=0x061738fb
		"fmsub.s f31, f15, f1, f12, dyn\n"                    // opcode=0x6017ffc7
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f0, f1\n"                                  // opcode=0x0610b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero






		"fswizz.ps f29, f13, 126\n"                           // opcode=0xe7f6cefb
		"fsrai.pi f30, f13, 1\n"                              // opcode=0x4e36ff7b
		"fsgnjn.s f17, f15, f1\n"                             // opcode=0x201798d3
		"fmv.s.x f0, x11\n"                                   // opcode=0xf0058053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f30, f15, f0\n"                             // opcode=0x20079f53




		"fsll.pi f16, f1, f14\n"                              // opcode=0x06e0987b
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m3, x14, 26\n"                               // opcode=0x566741fb
		"fnmsub.s f1, f15, f13, f15, rdn\n"                   // opcode=0x78d7a0cb
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x16\n"                                      // opcode=0xd600087b
		"cubesgntc.ps f30, f15, f14\n"                        // opcode=0x88e7bf7b
		"maskand m3, m0, m0\n"                                // opcode=0x660071fb
		"cubeface.ps f18, f12, f0\n"                          // opcode=0x8806097b
		"fbcx.ps f18, x15\n"                                  // opcode=0x0007b90b
		"fsrai.pi f19, f15, 3\n"                              // opcode=0x4e67f9fb
		"fcvt.ps.pw f0, f14, dyn\n"                           // opcode=0xd007707b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un8.ps f24, f15\n"                              // opcode=0xd9378c7b
		"cubeface.ps f22, f13, f14\n"                         // opcode=0x88e68b7b
		"fnmadd.s f18, f0, f0, f12, dyn\n"                    // opcode=0x6000794f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f16, f13\n"                                  // opcode=0x5876887b
		"feq.ps f26, f14, f0\n"                               // opcode=0xa0072d7b
		"fsub.pi f19, f0, f13\n"                              // opcode=0x0ed009fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f28, f1, f14\n"                               // opcode=0x06e0ee7b
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f28, f1, f15\n"                              // opcode=0xa6f0be7b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f23, x13, dyn\n"                            // opcode=0xd006fbd3


		"flw.ps f28, 436(x11)\n"                              // opcode=0x1b45ae0b
		"maskpopc x0, m0\n"                                   // opcode=0x5200007b
		"fcvt.s.wu f1, x1, dyn\n"                             // opcode=0xd010f0d3
		"flw.ps f1,  32 (x4)\n"
		"fsub.pi f26, f12, f13\n"                             // opcode=0x0ed60d7b
		"fsrai.pi f22, f0, 4\n"                               // opcode=0x4e907b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.sn8.ps f17, f13\n"                              // opcode=0xd9b688fb
		"fle.pi f28, f13, f13\n"                              // opcode=0xa6d68e7b
		"fcvt.sn16.ps f29, f13\n"                             // opcode=0xd9968efb
		//RTLMIN-5479"fsin.ps f16, f13\n"                                  // opcode=0x5866887b
		"fnmsub.s f25, f15, f14, f0, dyn\n"                   // opcode=0x00e7fccb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"cubefaceidx.ps f25, f1, f13\n"                       // opcode=0x88d09cfb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f0, f0\n"                                // opcode=0xd890007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x24\n"                                      // opcode=0xd6000c7b
		"fmin.ps f24, f12, f14\n"                             // opcode=0x28e60c7b
		"fmvs.x.ps x23, f13, 5\n"                             // opcode=0xe056abfb
		"fbcx.ps f26, x1\n"                                   // opcode=0x0000bd0b


		"fsw.ps f0, 954(x11)\n"                               // opcode=0x3a076d0b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fand.pi f18, f0, f14\n"                              // opcode=0x06e0797b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m4, f15, f0\n"                               // opcode=0x3e07827b
		"fle.pi f31, f1, f0\n"                                // opcode=0xa6008ffb
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f17, f15, f0, dyn\n"                         // opcode=0x0007f8fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsq2 f13, 1760(x14)\n"                               // opcode=0x50d65aa7
		"fcvt.wu.s x29, f1, dyn\n"                            // opcode=0xc010fed3
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f29, f1, f0\n"                              // opcode=0x20008efb
		"flw.ps f1,  32 (x4)\n"
		"fcmov.ps f27, f12, f13, f14\n"                       // opcode=0x74d62dbf
		"cubefaceidx.ps f26, f13, f1\n"                       // opcode=0x88169d7b
		"fcvt.f16.ps f16, f13\n"                              // opcode=0xd896887b
		"fbci.pi f31, 206735\n"                               // opcode=0x64f1ffdf




		"maskor m5, m4, m2\n"                                 // opcode=0x662262fb
		"fbci.pi f16, 130336\n"                               // opcode=0x3fa4185f
		"flt.ps f23, f0, f12\n"                               // opcode=0xa0c01bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"masknot m2, m6\n"                                    // opcode=0x6603217b
		"fsgnjn.ps f26, f0, f13\n"                            // opcode=0x20d01d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f22, -1052(x14)\n"                               // opcode=0xbe472b07
		"fsgnjn.s f27, f13, f14\n"                            // opcode=0x20e69dd3
		"fpackrepb.pi f28, f0\n"                              // opcode=0x26000e7b
		"fsat8.pi f25, f12\n"                                 // opcode=0x06063cfb
		"fmulh.pi f1, f0, f15\n"                              // opcode=0x16f020fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubesgntc.ps f0, f15, f14\n"                         // opcode=0x88e7b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f30, f13, f0\n"                       // opcode=0x88069f7b
		"fcvt.un24.ps f1, f12\n"                              // opcode=0xd90600fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f27, x13, dyn\n"                            // opcode=0xd006fdd3
		"fnmadd.ps f28, f12, f14, f12, dyn\n"                 // opcode=0x66e67e5b


		"flq2 f1, 1824(x15)\n"                                // opcode=0x76f65087
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f22, f0, dyn\n"                          // opcode=0xd0107b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f25, f1\n"                                 // opcode=0xe0009cfb


		"fbc.ps f31, 1472(x13)\n"                             // opcode=0x73c00f8b
		"fmv.s.x f17, x12\n"                                  // opcode=0xf00608d3
		"fsub.s f18, f0, f13, dyn\n"                          // opcode=0x08d07953
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.ps f21, f0, f12, dyn\n"                         // opcode=0x00c07afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f18, f15\n"                                  // opcode=0x5887897b
		"fmax.ps f31, f15, f1\n"                              // opcode=0x28179ffb
		"fle.ps f30, f15, f14\n"                              // opcode=0xa0e78f7b
		"fround.ps f18, f15, dyn\n"                           // opcode=0x5817f97b




		"fcvt.wu.s x25, f12, dyn\n"                           // opcode=0xc0167cd3
		"fsgnj.s f21, f0, f14\n"                              // opcode=0x20e00ad3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f19, f15, 68\n"                            // opcode=0xe71789fb
		"flt.s x29, f0, f13\n"                                // opcode=0xa0d01ed3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f24, f12, f13\n"                             // opcode=0x0ed65c7b
		"fmul.ps f25, f14, f12, dyn\n"                        // opcode=0x10c77cfb

		"li x5, 0xf27ad870cf3f2949\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fle.ps f22, f13, f0\n"                               // opcode=0xa0068b7b
		"fadd.ps f17, f13, f14, dyn\n"                        // opcode=0x00e6f8fb
		"fmax.ps f1, f1, f14\n"                               // opcode=0x28e090fb
		"flw.ps f1,  32 (x4)\n"
		"fmin.pi f22, f0, f1\n"                               // opcode=0x2e100b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f28, f13, f15\n"                             // opcode=0x20f68e53
		"fle.pi f16, f14, f0\n"                               // opcode=0xa607087b
		"fsat8.pi f25, f1\n"                                  // opcode=0x0600bcfb
		"masknot m6, m0\n"                                    // opcode=0x6600237b
		"fcvt.un10.ps f21, f0\n"                              // opcode=0xd9200afb
		"fmvz.x.ps x31, f13, 0\n"                             // opcode=0xe0068ffb
		"fbcx.ps f30, x1\n"                                   // opcode=0x0000bf0b
		"fsrai.pi f24, f12, 7\n"                              // opcode=0x4ef67c7b
		"flt.pi f29, f15, f15\n"                              // opcode=0xa6f79efb
		"fsub.s f25, f12, f1, dyn\n"                          // opcode=0x08167cd3
		"flw.ps f1,  32 (x4)\n"
		"fpackreph.pi f19, f1\n"                              // opcode=0x260099fb
		"ffrc.ps f28, f13\n"                                  // opcode=0x58268e7b
		"fminu.pi f17, f1, f1\n"                              // opcode=0x2e10a8fb
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f12, 30(x11)\n"                               // opcode=0x00c06f0b
		"flog.ps f1, f15\n"                                   // opcode=0x583780fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un2.ps f17, f0\n"                               // opcode=0xd97008fb


		"fsw.ps f13, -417(x14)\n"                             // opcode=0xe4d76f8b
		"fbci.pi f24, 350125\n"                               // opcode=0xaaf5ac5f
		"fcvt.ps.pwu f18, f14, dyn\n"                         // opcode=0xd017797b
		"fclass.s x29, f14\n"                                 // opcode=0xe0071ed3
		"fsgnjx.s f29, f13, f1\n"                             // opcode=0x2016aed3
		"fnot.pi f31, f15\n"                                  // opcode=0x0607affb
		"fcvt.un2.ps f26, f1\n"                               // opcode=0xd9708d7b
		"fmsub.ps f22, f1, f13, f1, dyn\n"                    // opcode=0x0ad0fb5b
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f23, f14, f12\n"                              // opcode=0xa0c72bfb
		"fcvt.ps.pw f27, f15, dyn\n"                          // opcode=0xd007fdfb
		"fsgnjn.ps f22, f12, f1\n"                            // opcode=0x20161b7b


		"flw.ps f16, 931(x11)\n"                              // opcode=0x3a36280b
		"maskpopc x23, m4\n"                                  // opcode=0x52020bfb
		"fbcx.ps f27, x13\n"                                  // opcode=0x0006bd8b
		"fmv.x.s x29, f1\n"                                   // opcode=0xe0008ed3
		"fcvt.wu.s x26, f1, dyn\n"                            // opcode=0xc010fd53
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f15, 312(x11)\n"                              // opcode=0x12f6ec0b




		"fltm.pi m1, f13, f12\n"                              // opcode=0x3ec680fb
		"for.pi f0, f12, f14\n"                               // opcode=0x06e6607b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m4, m6, m5\n"                                // opcode=0x6653427b


		"flq2 f18, 128(x12)\n"                                // opcode=0x19d7d907
		"fnmadd.ps f19, f12, f0, f0, dyn\n"                   // opcode=0x060679db
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.pi f24, f15, f14\n"                             // opcode=0x2ee79c7b
		"faddi.pi f23, f12, 154\n"                            // opcode=0x4d560bbf
		"fmin.pi f26, f1, f14\n"                              // opcode=0x2ee08d7b
		"flw.ps f1,  32 (x4)\n"
		"fcmov.ps f24, f14, f12, f1\n"                        // opcode=0x0cc72c3f








		"flt.ps f25, f13, f0\n"                               // opcode=0xa0069cfb


		"fsw f14, -997(x14)\n"                                // opcode=0xc0e72da7
		"fcvt.un24.ps f1, f0\n"                               // opcode=0xd90000fb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f26, f0, f0\n"                             // opcode=0x20002d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsatu8.pi f21, f0\n"                                 // opcode=0x06103afb
		"fcvt.un10.ps f28, f13\n"                             // opcode=0xd9268e7b
		"for.pi f17, f13, f1\n"                               // opcode=0x0616e8fb
		"cubesgntc.ps f22, f0, f12\n"                         // opcode=0x88c03b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f18, f13, 53\n"                            // opcode=0xe6d6b97b
		"fcvt.pw.ps f1, f1, rtz\n"                            // opcode=0xc00090fb
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f26, f0, f15\n"                              // opcode=0xa6f03d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m4, f12\n"                                  // opcode=0xe006527b


		"maskor m6, m3, m1\n"                                 // opcode=0x6611e37b
		"fadd.s f22, f14, f13, dyn\n"                         // opcode=0x00d77b53


		"flw.ps f25, 362(x15)\n"                              // opcode=0x16a72c8b


		"fsw f14, -1940(x11)\n"                               // opcode=0x86e5a627
		"fsgnj.ps f18, f1, f13\n"                             // opcode=0x20d0897b
		"flw.ps f1,  32 (x4)\n"


		"flq2 f29, 1696(x14)\n"                               // opcode=0x5c67de87
		"fand.pi f28, f13, f12\n"                             // opcode=0x06c6fe7b
		"fnot.pi f27, f0\n"                                   // opcode=0x06002dfb
		"fnmsub.ps f29, f1, f0, f14, dyn\n"                   // opcode=0x7400fedb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fsq2 f12, -288(x11)\n"                               // opcode=0x80c5d9a7
		"fcvt.un2.ps f30, f0\n"                               // opcode=0xd9700f7b
		"maskand m3, m1, m7\n"                                // opcode=0x6670f1fb
		"maskxor m7, m5, m7\n"                                // opcode=0x6672c3fb
		"fcvt.ps.pw f1, f0, dyn\n"                            // opcode=0xd00070fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x16, m1\n"                                  // opcode=0x5200887b
		"faddi.pi f19, f15, 343\n"                            // opcode=0xacf789bf
		"fcvt.ps.f16 f21, f15\n"                              // opcode=0xd0a78afb
		"maskand m2, m7, m4\n"                                // opcode=0x6643f17b
		"fpackrepb.pi f30, f12\n"                             // opcode=0x26060f7b
		"fmadd.s f18, f15, f15, f14, dyn\n"                   // opcode=0x70f7f943
		"fbcx.ps f31, x14\n"                                  // opcode=0x00073f8b
		"feq.s x23, f14, f12\n"                               // opcode=0xa0c72bd3
		"fcvt.w.s x23, f13, dyn\n"                            // opcode=0xc006fbd3
		"fpackreph.pi f28, f15\n"                             // opcode=0x26079e7b




		"fclass.s x16, f1\n"                                  // opcode=0xe0009853


		"fswizz.ps f26, f12, 10\n"                            // opcode=0xe6265d7b
		"flog.ps f18, f12\n"                                  // opcode=0x5836097b


		"fbci.pi f0, 414289\n"                                // opcode=0xca4a205f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.ps f31, f12, f13, dyn\n"                        // opcode=0x00d67ffb
		"fmvs.x.ps x17, f12, 3\n"                             // opcode=0xe03628fb
		"fexp.ps f31, f0\n"                                   // opcode=0x58400ffb
		"fsrl.pi f30, f12, f1\n"                              // opcode=0x06165f7b
		"maskpopc x31, m2\n"                                  // opcode=0x52010ffb


		"flw.ps f17, -34(x11)\n"                              // opcode=0xfde5a88b




		"fbci.pi f26, 359334\n"                               // opcode=0xaf74cd5f
		"mov.m.x m7, x14, 79\n"                               // opcode=0x573763fb
		"flt.s x31, f0, f13\n"                                // opcode=0xa0d01fd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.pi f26, f12, f1\n"                               // opcode=0xa6160d7b
		"fsgnj.ps f18, f0, f13\n"                             // opcode=0x20d0097b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m4, m7, m1\n"                                // opcode=0x6613c27b


		"fsq2 f0, 256(x14)\n"                                 // opcode=0x4600de27
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f23, f0, f13\n"                             // opcode=0x16d02bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xb519c4e2cbc27e00\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsat8.pi f21, f13\n"                                 // opcode=0x0606bafb
		"fminu.pi f18, f15, f14\n"                            // opcode=0x2ee7a97b


		"fsw f0, -1379(x15)\n"                                // opcode=0xa807aea7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f23, x15, rup\n"                            // opcode=0xd007bbd3
		"fbci.ps f0, 483119\n"                                // opcode=0xebe5e01f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.ps f19, f12, f13\n"                              // opcode=0xa0d619fb
		"fmaxu.pi f19, f15, f0\n"                             // opcode=0x2e07b9fb
		"fcvt.un2.ps f27, f1\n"                               // opcode=0xd9708dfb




		"fcvt.pwu.ps f25, f0, dyn\n"                          // opcode=0xc0107cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x31, f14, dyn\n"                           // opcode=0xc0177fd3
		"mov.m.x m6, x15, 19\n"                               // opcode=0x5647f37b
		"fxor.pi f1, f0, f0\n"                                // opcode=0x060040fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f25, f1, 83\n"                             // opcode=0xe740fcfb
		"flw.ps f1,  32 (x4)\n"


		"flw.ps f25, 1201(x14)\n"                             // opcode=0x4b15ac8b
		"fltm.ps m7, f1, f14\n"                               // opcode=0xa0e0d3fb
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f22, f12\n"                                  // opcode=0x58860b7b
		"fltu.pi f21, f12, f14\n"                             // opcode=0xa6e63afb
		"fmin.s f16, f13, f1\n"                               // opcode=0x28168853
		"fbcx.ps f25, x12\n"                                  // opcode=0x00063c8b
		"fcvt.sn8.ps f30, f13\n"                              // opcode=0xd9b68f7b
		"fsgnjn.s f16, f0, f15\n"                             // opcode=0x20f01853
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f19, x11, dyn\n"                            // opcode=0xd005f9d3
		"maskxor m2, m5, m3\n"                                // opcode=0x6632c17b
		"ffrc.ps f23, f15\n"                                  // opcode=0x58278bfb
		"fcmov.ps f21, f15, f13, f0\n"                        // opcode=0x04d7aabf
		"cubesgntc.ps f30, f1, f14\n"                         // opcode=0x88e0bf7b
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f18, f15, f0\n"                          // opcode=0x8807897b
		"fxor.pi f0, f1, f0\n"                                // opcode=0x0600c07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f31, f14, rtz\n"                          // opcode=0xc0071ffb
		"fsat8.pi f29, f13\n"                                 // opcode=0x0606befb
		"fmax.ps f20, f15, f1\n"                              // opcode=0x28179a7b
		"fltu.pi f17, f0, f0\n"                               // opcode=0xa60038fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x23, f0, dyn\n"                            // opcode=0xc0107bd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f1, f0\n"                                    // opcode=0x587000fb
		"flw.ps f1,  32 (x4)\n"
		"ffrc.ps f31, f0\n"                                   // opcode=0x58200ffb
		"fnmadd.ps f1, f13, f14, f0, dyn\n"                   // opcode=0x06e6f0db
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskxor m3, m0, m3\n"                                // opcode=0x663041fb
		"for.pi f20, f1, f15\n"                               // opcode=0x06f0ea7b
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f26, f0, 14\n"                              // opcode=0x4fd05d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f0, f13, f14\n"                              // opcode=0xa6e6b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f17, f0, f0, f1, dyn\n"                      // opcode=0x080078c7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x25, f14\n"                                 // opcode=0xe0071cd3
		"fsgnjx.s f20, f12, f1\n"                             // opcode=0x20162a53
		"cubefaceidx.ps f30, f0, f0\n"                        // opcode=0x88001f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f28, f13, 9\n"                              // opcode=0x4f36de7b


		"fsw.ps f15, 416(x14)\n"                              // opcode=0x1af7600b


		"fswizz.ps f16, f13, 74\n"                            // opcode=0xe726d87b
		"fnmsub.ps f22, f1, f13, f14, dyn\n"                  // opcode=0x74d0fb5b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x27, f1, rmm\n"                             // opcode=0xc000cdd3
		"flw.ps f1,  32 (x4)\n"
		"fsub.s f21, f15, f14, dyn\n"                         // opcode=0x08e7fad3

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsub.s f16, f12, f15, dyn\n"                         // opcode=0x08f67853
		"fclass.ps f24, f15\n"                                // opcode=0xe0079c7b
		"fbci.pi f29, 456094\n"                               // opcode=0xdeb3cedf
		"fsat8.pi f26, f1\n"                                  // opcode=0x0600bd7b
		"fclass.ps f30, f12\n"                                // opcode=0xe0061f7b
		"fcvt.un10.ps f17, f15\n"                             // opcode=0xd92788fb
		"fmsub.s f17, f12, f15, f14, dyn\n"                   // opcode=0x70f678c7




		"fnmsub.s f18, f0, f15, f13, rdn\n"                   // opcode=0x68f0294b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f16, f0\n"                                   // opcode=0x5820087b
		"fsgnjn.s f17, f12, f15\n"                            // opcode=0x20f618d3
		"fmvz.x.ps x21, f15, 4\n"                             // opcode=0xe0478afb
		"maskand m1, m7, m7\n"                                // opcode=0x6673f0fb




		"flt.ps f28, f13, f1\n"                               // opcode=0xa0169e7b
		"fcvt.un10.ps f29, f15\n"                             // opcode=0xd9278efb
		"fmul.s f0, f13, f12, dyn\n"                          // opcode=0x10c6f053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f20, 473886\n"                               // opcode=0xe763da1f
		"fmsub.ps f19, f0, f1, f15, dyn\n"                    // opcode=0x7a1079db
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f17, f12, rmm\n"                         // opcode=0xd01648fb
		"fminu.pi f1, f13, f1\n"                              // opcode=0x2e16a0fb
		"flw.ps f1,  32 (x4)\n"




		"fsgnj.ps f28, f0, f15\n"                             // opcode=0x20f00e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrl.pi f20, f15, f15\n"                             // opcode=0x06f7da7b
		"fbci.ps f17, 191271\n"                               // opcode=0x5d64f89f




		"fmvs.x.ps x27, f1, 0\n"                              // opcode=0xe000adfb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f30, f15, f1\n"                            // opcode=0x2017af7b
		"fsra.pi f31, f13, f12\n"                             // opcode=0x0ec6dffb
		"fsrl.pi f1, f0, f1\n"                                // opcode=0x061050fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f1, f15, f13\n"                          // opcode=0x88d780fb
		"flw.ps f1,  32 (x4)\n"
		"fnmsub.s f0, f15, f14, f0, dyn\n"                    // opcode=0x00e7f04b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"flog.ps f30, f1\n"                                   // opcode=0x58308f7b
		"fmax.pi f26, f12, f1\n"                              // opcode=0x2e161d7b
		"cubefaceidx.ps f22, f12, f15\n"                      // opcode=0x88f61b7b
		"fmax.s f0, f12, f15\n"                               // opcode=0x28f61053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f31, f0, dyn\n"                           // opcode=0xd0007ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un16.ps f29, f12\n"                             // opcode=0xd9160efb
		"fsgnj.ps f30, f0, f12\n"                             // opcode=0x20c00f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x30, f13\n"                                  // opcode=0xe0068f53
		"fcvt.ps.pwu f19, f13, dyn\n"                         // opcode=0xd016f9fb
		"fcvt.f16.ps f21, f14\n"                              // opcode=0xd8970afb
		"fadd.pi f23, f0, f13\n"                              // opcode=0x06d00bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flog.ps f16, f0\n"                                   // opcode=0x5830087b
		"fcvt.ps.pw f22, f12, dyn\n"                          // opcode=0xd0067b7b
		"fandi.pi f0, f0, 254\n"                              // opcode=0x7dd0103f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.ps f19, f13, f14, dyn\n"                        // opcode=0x00e6f9fb
		"feq.s x1, f0, f1\n"                                  // opcode=0xa01020d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f20, f14, rne\n"                         // opcode=0xc0170a7b
		"fsgnj.ps f27, f0, f1\n"                              // opcode=0x20100dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fround.ps f16, f1, dyn\n"                            // opcode=0x5810f87b
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f30, f14, f13, dyn\n"                        // opcode=0x00d77f7b
		//RTLMIN-5479"fsin.ps f27, f14\n"                                  // opcode=0x58670dfb
		"fle.ps f25, f14, f0\n"                               // opcode=0xa0070cfb

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"ffrc.ps f31, f15\n"                                  // opcode=0x58278ffb
		"fnmsub.s f23, f12, f14, f15, dyn\n"                  // opcode=0x78e67bcb
		"fsrl.pi f28, f12, f12\n"                             // opcode=0x06c65e7b




		"fcvt.sn8.ps f31, f13\n"                              // opcode=0xd9b68ffb
		"flt.ps f20, f14, f12\n"                              // opcode=0xa0c71a7b
		"fmv.x.s x29, f14\n"                                  // opcode=0xe0070ed3
		"fsatu8.pi f23, f13\n"                                // opcode=0x0616bbfb
		"fmulhu.pi f28, f13, f0\n"                            // opcode=0x16069e7b
		"fbcx.ps f30, x13\n"                                  // opcode=0x0006bf0b
		"flt.s x30, f15, f15\n"                               // opcode=0xa0f79f53
		"fslli.pi f26, f13, 9\n"                              // opcode=0x4f269d7b
		"fmul.pi f16, f12, f0\n"                              // opcode=0x1606087b
		"fround.ps f18, f1, dyn\n"                            // opcode=0x5810f97b
		"flw.ps f1,  32 (x4)\n"
		"fexp.ps f20, f13\n"                                  // opcode=0x58468a7b


		"flw.ps f28, 679(x15)\n"                              // opcode=0x2a762e0b
		"fbcx.ps f24, x14\n"                                  // opcode=0x00073c0b
		"fsrl.pi f29, f15, f0\n"                              // opcode=0x0607defb
		"mova.x.m x25\n"                                      // opcode=0xd6000cfb
		"fmaxu.pi f31, f12, f14\n"                            // opcode=0x2ee63ffb
		"fsgnjn.ps f23, f12, f0\n"                            // opcode=0x20061bfb
		"fmvs.x.ps x1, f13, 0\n"                              // opcode=0xe006a0fb
		"fle.pi f20, f14, f1\n"                               // opcode=0xa6170a7b
		"fcvt.w.s x22, f15, dyn\n"                            // opcode=0xc007fb53
		"fadd.ps f24, f12, f12, dyn\n"                        // opcode=0x00c67c7b
		"fcmov.ps f31, f13, f0, f13\n"                        // opcode=0x6c06afbf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.sn8.ps f17, f13\n"                              // opcode=0xd9b688fb
		"fsgnjn.ps f21, f12, f14\n"                           // opcode=0x20e61afb
		"cubeface.ps f29, f1, f1\n"                           // opcode=0x88108efb
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x16, f0\n"                                  // opcode=0xe0001853
		"fsra.pi f21, f0, f13\n"                              // opcode=0x0ed05afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.ps f29, f14, f14, dyn\n"                        // opcode=0x08e77efb
		"maskpopc x25, m6\n"                                  // opcode=0x52030cfb
		"fmsub.s f1, f13, f15, f14, dyn\n"                    // opcode=0x70f6f0c7
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.s f27, f14, f0\n"                             // opcode=0x20071dd3
		"fsrli.pi f25, f12, 2\n"                              // opcode=0x4e565cfb
		"maskpopc x16, m1\n"                                  // opcode=0x5200887b
		"fsetm.pi m1, f13\n"                                  // opcode=0xe006d0fb
		"maskpopc x21, m4\n"                                  // opcode=0x52020afb
		"fsat8.pi f20, f1\n"                                  // opcode=0x0600ba7b
		"feq.pi f25, f15, f13\n"                              // opcode=0xa6d7acfb
		"fcvt.un16.ps f28, f14\n"                             // opcode=0xd9170e7b
		"fmul.ps f26, f1, f0, dyn\n"                          // opcode=0x1000fd7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fnot.pi f1, f1\n"                                    // opcode=0x0600a0fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.sn8.ps f0, f1\n"                                // opcode=0xd9b0807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un16.ps f22, f12\n"                             // opcode=0xd9160b7b
		//RTLMIN-5479"frsq.ps f24, f0\n"                                   // opcode=0x58800c7b
		"fcvt.un10.ps f23, f0\n"                              // opcode=0xd9200bfb
		"fsat8.pi f0, f1\n"                                   // opcode=0x0600b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmov.ps f31, f13, f13, f0\n"                        // opcode=0x04d6afbf
		"fsub.pi f26, f1, f13\n"                              // opcode=0x0ed08d7b
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsatu8.pi f22, f14\n"                                // opcode=0x06173b7b
		"fsgnj.ps f31, f12, f12\n"                            // opcode=0x20c60ffb
		"feq.s x19, f12, f1\n"                                // opcode=0xa01629d3
		"fle.pi f16, f0, f15\n"                               // opcode=0xa6f0087b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.s.x f31, x15\n"                                  // opcode=0xf0078fd3
		"ffrc.ps f29, f0\n"                                   // opcode=0x58200efb
		"cubesgnsc.ps f25, f15, f0\n"                         // opcode=0x8807acfb
		"fcmov.ps f31, f12, f0, f15\n"                        // opcode=0x7c062fbf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x27\n"                                      // opcode=0xd6000dfb
		"fnmadd.s f0, f15, f12, f13, dyn\n"                   // opcode=0x68c7f04f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f19, -928(x11)\n"                             // opcode=0xebb5898b
		"fmvz.x.ps x20, f15, 0\n"                             // opcode=0xe0078a7b
		"fmvs.x.ps x30, f13, 7\n"                             // opcode=0xe076af7b
		"fcvt.sn8.ps f31, f0\n"                               // opcode=0xd9b00ffb
		"fbci.pi f1, 248577\n"                                // opcode=0x796030df
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f29, f13, f0\n"                              // opcode=0xa606befb
		"fsub.ps f17, f0, f15, dyn\n"                         // opcode=0x08f078fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f30, f13, 13\n"                             // opcode=0x4fb6ff7b


		"fsw f1, 1499(x13)\n"                                 // opcode=0x5c102da7
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x23, f13\n"                                  // opcode=0xe0068bd3
		"fbcx.ps f20, x1\n"                                   // opcode=0x0000ba0b
		"fltm.ps m1, f14, f12\n"                              // opcode=0xa0c750fb
		"fbcx.ps f1, x12\n"                                   // opcode=0x0006308b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f17, x14, dyn\n"                           // opcode=0xd01778d3
		"fcvt.pwu.ps f21, f14, rmm\n"                         // opcode=0xc0174afb
		"fmadd.ps f20, f12, f14, f12, dyn\n"                  // opcode=0x60e67a5b
		"cubesgnsc.ps f18, f1, f15\n"                         // opcode=0x88f0a97b
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f25, f12, f13\n"                           // opcode=0x20d61cfb
		"fnmadd.ps f21, f1, f14, f14, dyn\n"                  // opcode=0x76e0fadb
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f28, f15, f13\n"                      // opcode=0x88d79e7b
		"masknot m7, m0\n"                                    // opcode=0x660023fb
		"fle.pi f25, f14, f13\n"                              // opcode=0xa6d70cfb
		"fmsub.ps f18, f0, f14, f14, dyn\n"                   // opcode=0x72e0795b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"maskor m0, m2, m1\n"                                 // opcode=0x6611607b
		"maskor m2, m6, m4\n"                                 // opcode=0x6643617b
		"fmadd.s f1, f13, f1, f0, dyn\n"                      // opcode=0x0016f0c3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f28, f13\n"                                  // opcode=0x58868e7b
		//RTLMIN-5479"frsq.ps f1, f0\n"                                    // opcode=0x588000fb
		"flw.ps f1,  32 (x4)\n"
		"fmvs.x.ps x23, f14, 7\n"                             // opcode=0xe0772bfb
		"fcvt.ps.f16 f21, f13\n"                              // opcode=0xd0a68afb


		"fswizz.ps f18, f12, 71\n"                            // opcode=0xe716697b
		"fsat8.pi f31, f14\n"                                 // opcode=0x06073ffb
		"fltm.ps m1, f13, f12\n"                              // opcode=0xa0c6d0fb
		"fmulhu.pi f22, f13, f1\n"                            // opcode=0x16169b7b
		"fsgnj.ps f16, f14, f15\n"                            // opcode=0x20f7087b




		"ffrc.ps f28, f12\n"                                  // opcode=0x58260e7b
		"masknot m5, m2\n"                                    // opcode=0x660122fb
		"fcvt.un24.ps f1, f12\n"                              // opcode=0xd90600fb
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f17, x11\n"                                  // opcode=0xf00588d3
		"fcmovm.ps f21, f14, f12\n"                           // opcode=0x00c70af7

		"li x5, 0x86ef5ddf733ccfb4\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fswizz.ps f0, f14, 101\n"                            // opcode=0xe797207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f27, f12\n"                                // opcode=0xe0061dfb
		"fcvt.ps.pwu f21, f14, dyn\n"                         // opcode=0xd0177afb


		"flw.ps f30, -792(x14)\n"                             // opcode=0xce872f0b
		"fsrl.pi f22, f14, f15\n"                             // opcode=0x06f75b7b
		"fcvt.f16.ps f22, f14\n"                              // opcode=0xd8970b7b
		"fle.pi f21, f12, f14\n"                              // opcode=0xa6e60afb
		"flt.s x27, f12, f14\n"                               // opcode=0xa0e61dd3


		"fsw f13, 1605(x11)\n"                                // opcode=0x64d0a2a7
		"fmax.pi f23, f0, f15\n"                              // opcode=0x2ef01bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f0, 227972\n"                                // opcode=0x6f50801f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f16, f14, dyn\n"                         // opcode=0xd017787b
		"flt.pi f22, f1, f13\n"                               // opcode=0xa6d09b7b
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f29, f12, f14\n"                           // opcode=0x00e60ef7
		"maskxor m7, m7, m4\n"                                // opcode=0x6643c3fb
		"maskxor m5, m4, m4\n"                                // opcode=0x664242fb
		"mova.x.m x22\n"                                      // opcode=0xd6000b7b
		"fsetm.pi m4, f12\n"                                  // opcode=0xe006527b
		"maskpopcz x30, m6\n"                                 // opcode=0x54030f7b
		"cubesgntc.ps f1, f15, f12\n"                         // opcode=0x88c7b0fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f18, f13, dyn\n"                         // opcode=0xd016f97b
		"cubesgnsc.ps f22, f13, f13\n"                        // opcode=0x88d6ab7b
		"fexp.ps f24, f0\n"                                   // opcode=0x58400c7b
		"fclass.s x18, f1\n"                                  // opcode=0xe0009953
		"mova.x.m x17\n"                                      // opcode=0xd60008fb
		"masknot m2, m6\n"                                    // opcode=0x6603217b
		"fxor.pi f17, f12, f1\n"                              // opcode=0x061648fb
		"fmulh.pi f25, f14, f0\n"                             // opcode=0x16072cfb
		"maskxor m1, m4, m2\n"                                // opcode=0x662240fb
		"fsrai.pi f21, f1, 15\n"                              // opcode=0x4ff0fafb
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"fsin.ps f20, f12\n"                                  // opcode=0x58660a7b
		"fsrli.pi f31, f0, 0\n"                               // opcode=0x4e105ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fslli.pi f31, f0, 14\n"                              // opcode=0x4fd01ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.pi f18, f12, f0\n"                              // opcode=0x1606097b
		"fcvt.f16.ps f31, f12\n"                              // opcode=0xd8960ffb
		"fbci.pi f21, 380646\n"                               // opcode=0xb9dcdadf


		"fbc.ps f17, -992(x11)\n"                             // opcode=0xa935888b
		"fle.pi f1, f12, f15\n"                               // opcode=0xa6f600fb
		"flw.ps f1,  32 (x4)\n"
		"maskand m1, m1, m3\n"                                // opcode=0x6630f0fb
		"fmulh.pi f25, f1, f15\n"                             // opcode=0x16f0acfb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f20, f14, dyn\n"                         // opcode=0xd0177a7b
		"fminu.pi f1, f12, f14\n"                             // opcode=0x2ee620fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x18, f1, dyn\n"                             // opcode=0xc000f953
		"flw.ps f1,  32 (x4)\n"
		"for.pi f1, f12, f1\n"                                // opcode=0x061660fb
		"flw.ps f1,  32 (x4)\n"
		"maskxor m7, m0, m2\n"                                // opcode=0x662043fb
		"fminu.pi f1, f12, f13\n"                             // opcode=0x2ed620fb
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f29, f15, f12\n"                              // opcode=0xa6c79efb


		"flw.ps f26, -477(x11)\n"                             // opcode=0xe235ad0b
		"fcvt.un24.ps f19, f14\n"                             // opcode=0xd90709fb
		"fcmov.ps f0, f1, f1, f1\n"                           // opcode=0x0c10a03f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsrli.pi f18, f14, 8\n"                              // opcode=0x4f07597b
		"fpackrepb.pi f24, f13\n"                             // opcode=0x26068c7b
		"ffrc.ps f27, f14\n"                                  // opcode=0x58270dfb
		"fmv.x.s x30, f15\n"                                  // opcode=0xe0078f53
		//RTLMIN-5479"frsq.ps f22, f0\n"                                   // opcode=0x58800b7b
		"fcvt.s.wu f27, x11, dyn\n"                           // opcode=0xd015fdd3
		"fnot.pi f30, f0\n"                                   // opcode=0x06002f7b
		"mova.x.m x26\n"                                      // opcode=0xd6000d7b
		"fcvt.un24.ps f16, f0\n"                              // opcode=0xd900087b
		"fbci.pi f16, 141597\n"                               // opcode=0x4523b85f
		"mov.m.x m1, x13, 122\n"                              // opcode=0x57e6d0fb
		"fltm.ps m5, f0, f0\n"                                // opcode=0xa00052fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmov.ps f26, f12, f14, f1\n"                        // opcode=0x0ce62d3f


		"flw.ps f17, 883(x13)\n"                              // opcode=0x3730288b
		"fcvt.s.w f25, x13, dyn\n"                            // opcode=0xd006fcd3


		"flw.ps f23, -21(x12)\n"                              // opcode=0xfeb62b8b
		"fsgnjx.ps f1, f14, f13\n"                            // opcode=0x20d720fb
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f19, f14, f1\n"                               // opcode=0xa01719fb
		"fcmovm.ps f17, f15, f0\n"                            // opcode=0x000788f7
		"fcvt.pw.ps f27, f14, dyn\n"                          // opcode=0xc0077dfb
		"fbcx.ps f27, x11\n"                                  // opcode=0x0005bd8b
		"fexp.ps f26, f15\n"                                  // opcode=0x58478d7b
		"fcvt.un2.ps f0, f0\n"                                // opcode=0xd970007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x18, m0\n"                                  // opcode=0x5200097b
		"fnot.pi f20, f1\n"                                   // opcode=0x0600aa7b




		"fcvt.s.w f22, x13, dyn\n"                            // opcode=0xd006fb53
		"cubeface.ps f19, f15, f12\n"                         // opcode=0x88c789fb
		"maskand m4, m1, m5\n"                                // opcode=0x6650f27b


		"fbc.ps f24, 1408(x14)\n"                             // opcode=0x65958c0b


		"fsw f15, -1298(x11)\n"                               // opcode=0xaef5a727
		"frcp.ps f27, f1\n"                                   // opcode=0x58708dfb
		"fcvt.un2.ps f1, f0\n"                                // opcode=0xd97000fb
		"flw.ps f1,  32 (x4)\n"
		"cubesgnsc.ps f18, f15, f12\n"                        // opcode=0x88c7a97b
		"fmsub.ps f28, f12, f12, f13, rdn\n"                  // opcode=0x6ac62e5b


		"fsw f12, 100(x14)\n"                                 // opcode=0x06c5a227
		"fsrl.pi f21, f15, f1\n"                              // opcode=0x0617dafb
		"faddi.pi f24, f13, 227\n"                            // opcode=0x74668c3f
		"fsgnj.ps f17, f12, f15\n"                            // opcode=0x20f608fb
		"fnmadd.s f1, f1, f1, f12, dyn\n"                     // opcode=0x6010f0cf
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f28, f0\n"                                   // opcode=0x58800e7b
		"fcvt.pwu.ps f30, f14, dyn\n"                         // opcode=0xc0177f7b


		"fcvt.pwu.ps f17, f13, rne\n"                         // opcode=0xc01688fb
		"fcvt.ps.f16 f30, f14\n"                              // opcode=0xd0a70f7b
		"fadd.ps f21, f1, f12, dyn\n"                         // opcode=0x00c0fafb
		"flw.ps f1,  32 (x4)\n"
		"masknot m0, m6\n"                                    // opcode=0x6603207b
		//RTLMIN-5479"fsin.ps f29, f13\n"                                  // opcode=0x58668efb
		"flt.pi f21, f0, f12\n"                               // opcode=0xa6c01afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.pi f27, f13, f12\n"                             // opcode=0x2ec69dfb


		"flq2 f26, -1056(x14)\n"                              // opcode=0x8d275d07
		"fcvt.s.wu f16, x14, dyn\n"                           // opcode=0xd0177853

		"li x5, 0x4d34523e2129a29d\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fswizz.ps f0, f1, 13\n"                              // opcode=0xe630b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"masknot m0, m3\n"                                    // opcode=0x6601a07b


		"fsw f0, -316(x13)\n"                                 // opcode=0xec06a227
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmovm.ps f0, f14, f14\n"                            // opcode=0x00e70077
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fclass.s x19, f0\n"                                  // opcode=0xe00019d3
		"fpackrepb.pi f26, f14\n"                             // opcode=0x26070d7b
		"fadd.pi f26, f13, f12\n"                             // opcode=0x06c68d7b
		"fcvt.s.wu f19, x0, dyn\n"                            // opcode=0xd01079d3
		"fadd.pi f23, f12, f14\n"                             // opcode=0x06e60bfb






		"flw.ps f27, 45(x13)\n"                               // opcode=0x02d02d8b


		"flw f29, 520(x15)\n"                                 // opcode=0x2086ae87
		"fltm.pi m2, f15, f12\n"                              // opcode=0x3ec7817b
		"fsgnjn.ps f22, f1, f14\n"                            // opcode=0x20e09b7b
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f22, f0, 397\n"                             // opcode=0xc5b01b3f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f24, f12, 86\n"                            // opcode=0xe7565c7b
		"fsra.pi f22, f12, f14\n"                             // opcode=0x0ee65b7b
		"fsgnj.s f26, f15, f0\n"                              // opcode=0x20078d53
		"fsub.ps f31, f12, f14, dyn\n"                        // opcode=0x08e67ffb
		"fmax.pi f20, f12, f15\n"                             // opcode=0x2ef61a7b
		"fadd.ps f25, f1, f1, dyn\n"                          // opcode=0x0010fcfb
		"flw.ps f1,  32 (x4)\n"
		"fand.pi f17, f14, f12\n"                             // opcode=0x06c778fb




		"flt.s x21, f1, f0\n"                                 // opcode=0xa0009ad3
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f0, f12, 267\n"                             // opcode=0x8566103f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f17, f1, f12\n"                       // opcode=0x88c098fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f17, x1, dyn\n"                            // opcode=0xd010f8d3


		"flq2 f30, -1152(x12)\n"                              // opcode=0xee065f07




		"flt.pi f22, f13, f0\n"                               // opcode=0xa6069b7b
		"fcvt.un24.ps f26, f0\n"                              // opcode=0xd9000d7b
		//RTLMIN-5479"fsin.ps f22, f0\n"                                   // opcode=0x58600b7b
		"fexp.ps f17, f13\n"                                  // opcode=0x584688fb
		"fadd.ps f22, f14, f13, dyn\n"                        // opcode=0x00d77b7b
		"fround.ps f22, f12, dyn\n"                           // opcode=0x58167b7b
		"fcvt.ps.f16 f30, f14\n"                              // opcode=0xd0a70f7b
		"maskxor m1, m0, m6\n"                                // opcode=0x666040fb
		"fadd.pi f26, f0, f1\n"                               // opcode=0x06100d7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.w.s x29, f14, dyn\n"                            // opcode=0xc0077ed3
		"fnmsub.ps f22, f13, f0, f0, dyn\n"                   // opcode=0x0406fb5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.s f24, f15, f12, f0, dyn\n"                    // opcode=0x00c7fc43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un24.ps f31, f13\n"                             // opcode=0xd9068ffb
		"fclass.s x23, f13\n"                                 // opcode=0xe0069bd3
		"feq.ps f30, f0, f15\n"                               // opcode=0xa0f02f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f20, f15, f0, f0, dyn\n"                   // opcode=0x0607fa5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f20, f14, dyn\n"                          // opcode=0xd0077a7b
		"flog.ps f31, f15\n"                                  // opcode=0x58378ffb
		"fmaxu.pi f30, f15, f1\n"                             // opcode=0x2e17bf7b
		"fsgnjn.s f1, f0, f13\n"                              // opcode=0x20d010d3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fpackrepb.pi f30, f15\n"                             // opcode=0x26078f7b
		"fcvt.wu.s x28, f15, dyn\n"                           // opcode=0xc017fe53
		"fmax.pi f0, f13, f12\n"                              // opcode=0x2ec6907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f31, f12, f0\n"                       // opcode=0x88061ffb

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"mov.m.x m7, x1, 100\n"                               // opcode=0x579083fb
		"maskpopc x29, m1\n"                                  // opcode=0x52008efb
		"fcvt.s.w f31, x12, dyn\n"                            // opcode=0xd0067fd3




		"maskpopc x31, m4\n"                                  // opcode=0x52020ffb
		"feqm.ps m2, f14, f0\n"                               // opcode=0xa007617b
		"fmvs.x.ps x17, f0, 1\n"                              // opcode=0xe01028fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f21, f13, 79\n"                            // opcode=0xe736eafb
		"mova.m.x x14\n"                                      // opcode=0xd607107b
		"fmulh.pi f17, f13, f0\n"                             // opcode=0x1606a8fb
		"fmvz.x.ps x31, f12, 3\n"                             // opcode=0xe0360ffb
		"fmadd.s f27, f15, f14, f1, dyn\n"                    // opcode=0x08e7fdc3
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f20, f13\n"                                // opcode=0x0616ba7b
		"fmsub.s f21, f14, f14, f15, dyn\n"                   // opcode=0x78e77ac7
		"fcvt.un16.ps f28, f0\n"                              // opcode=0xd9100e7b
		"fadd.pi f1, f13, f0\n"                               // opcode=0x060680fb
		"flw.ps f1,  32 (x4)\n"
		"maskxor m5, m0, m3\n"                                // opcode=0x663042fb




		"fsgnjx.s f29, f14, f1\n"                             // opcode=0x20172ed3
		"fsatu8.pi f28, f15\n"                                // opcode=0x0617be7b
		"fmv.s.x f20, x15\n"                                  // opcode=0xf0078a53
		"mova.x.m x29\n"                                      // opcode=0xd6000efb
		"fexp.ps f27, f14\n"                                  // opcode=0x58470dfb
		"fsgnjx.ps f17, f1, f13\n"                            // opcode=0x20d0a8fb
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f25, f14, f12\n"                           // opcode=0x20c71cfb


		"fsw.ps f15, 1564(x12)\n"                             // opcode=0x60f7ee0b
		"fclass.s x17, f1\n"                                  // opcode=0xe00098d3
		"fltm.ps m0, f0, f13\n"                               // opcode=0xa0d0507b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f20, f13\n"                                  // opcode=0x58868a7b




		"flt.pi f25, f15, f12\n"                              // opcode=0xa6c79cfb
		"maskand m4, m4, m5\n"                                // opcode=0x6652727b
		"fcvt.sn8.ps f30, f12\n"                              // opcode=0xd9b60f7b
		"fltu.pi f27, f1, f13\n"                              // opcode=0xa6d0bdfb
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f30, -1728(x14)\n"                            // opcode=0xc1f08f0b
		"fmsub.ps f0, f0, f15, f1, rtz\n"                     // opcode=0x0af0105b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskxor m1, m1, m4\n"                                // opcode=0x6640c0fb
		"fmvz.x.ps x23, f12, 1\n"                             // opcode=0xe0160bfb
		"fltm.ps m0, f15, f13\n"                              // opcode=0xa0d7d07b
		"fmulhu.pi f21, f15, f14\n"                           // opcode=0x16e79afb
		"fmv.s.x f25, x11\n"                                  // opcode=0xf0058cd3
		"fltm.ps m0, f12, f14\n"                              // opcode=0xa0e6507b
		"fcvt.s.wu f24, x11, dyn\n"                           // opcode=0xd015fc53
		"fmul.ps f17, f15, f0, dyn\n"                         // opcode=0x1007f8fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.s x1, f12, f15\n"                                // opcode=0xa0f620d3
		"fexp.ps f30, f15\n"                                  // opcode=0x58478f7b
		"fcvt.w.s x19, f15, dyn\n"                            // opcode=0xc007f9d3
		"fadd.s f26, f15, f1, dyn\n"                          // opcode=0x0017fd53
		"flw.ps f1,  32 (x4)\n"
		"flem.ps m5, f0, f15\n"                               // opcode=0xa0f042fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m2, f15, f0\n"                               // opcode=0x3e07817b
		"maskpopcz x26, m1\n"                                 // opcode=0x54008d7b
		"maskpopc x29, m1\n"                                  // opcode=0x52008efb
		"mov.m.x m2, x1, 122\n"                               // opcode=0x57e0c17b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"frcp.ps f31, f12\n"                                  // opcode=0x58760ffb
		"fmulh.pi f0, f14, f14\n"                             // opcode=0x16e7207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f25, f12\n"                                // opcode=0xe0061cfb
		"fmin.ps f31, f0, f1\n"                               // opcode=0x28100ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.s f17, f15, f14, f15, rne\n"                   // opcode=0x78e788c3


		"fle.s x22, f15, f14\n"                               // opcode=0xa0e78b53
		"fmul.ps f31, f0, f13, rdn\n"                         // opcode=0x10d02ffb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f17, f1, f13\n"                              // opcode=0x0ed088fb
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f29, x15\n"                                  // opcode=0xf0078ed3
		"fcvt.ps.pw f28, f12, dyn\n"                          // opcode=0xd0067e7b
		"fmulh.pi f20, f1, f13\n"                             // opcode=0x16d0aa7b
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f17, -1312(x13)\n"                            // opcode=0xbb80088b
		"fpackrepb.pi f20, f14\n"                             // opcode=0x26070a7b
		"fmv.s.x f26, x14\n"                                  // opcode=0xf0070d53
		"fmul.ps f20, f1, f13, dyn\n"                         // opcode=0x10d0fa7b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.sn8.ps f20, f14\n"                              // opcode=0xd9b70a7b
		"fcvt.s.wu f17, x11, dyn\n"                           // opcode=0xd015f8d3
		"fsgnj.ps f27, f1, f13\n"                             // opcode=0x20d08dfb
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f24, f12, f13, f14, dyn\n"                   // opcode=0x70d67c43
		"fcvt.un16.ps f25, f12\n"                             // opcode=0xd9160cfb


		"fsq2 f0, -1344(x12)\n"                               // opcode=0x9c065327
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.ps m2, f13, f0\n"                               // opcode=0xa006d17b
		"fmadd.ps f18, f1, f0, f0, dyn\n"                     // opcode=0x0000f95b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f1, f15, f1, f0, dyn\n"                     // opcode=0x0217f0db
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fsatu8.pi f25, f14\n"                                // opcode=0x06173cfb
		"cubeface.ps f17, f1, f0\n"                           // opcode=0x880088fb
		"flw.ps f1,  32 (x4)\n"
		"fmulhu.pi f19, f13, f13\n"                           // opcode=0x16d699fb
		"fcvt.f16.ps f29, f12\n"                              // opcode=0xd8960efb
		"frcp.ps f28, f1\n"                                   // opcode=0x58708e7b
		"fsgnjn.s f17, f12, f1\n"                             // opcode=0x201618d3


		"flw f17, -1862(x12)\n"                               // opcode=0x8ba62887
		"fpackrepb.pi f23, f14\n"                             // opcode=0x26070bfb
		"maskand m2, m6, m0\n"                                // opcode=0x6603717b
		"fmulhu.pi f18, f14, f0\n"                            // opcode=0x1607197b
		"fcvt.un16.ps f29, f12\n"                             // opcode=0xd9160efb


		"fbc.ps f21, 704(x15)\n"                              // opcode=0x79660a8b
		"mov.m.x m3, x12, 108\n"                              // opcode=0x57b611fb
		"fmadd.s f31, f0, f14, f12, dyn\n"                    // opcode=0x60e07fc3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f24, f13\n"                                  // opcode=0x58268c7b
		"fle.pi f22, f15, f0\n"                               // opcode=0xa6078b7b
		"fmul.s f27, f12, f1, dyn\n"                          // opcode=0x10167dd3
		"flw.ps f1,  32 (x4)\n"
		"fbcx.ps f22, x14\n"                                  // opcode=0x00073b0b
		"fcvt.un2.ps f29, f15\n"                              // opcode=0xd9778efb
		"flt.s x20, f1, f14\n"                                // opcode=0xa0e09a53
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un8.ps f26, f15\n"                              // opcode=0xd9378d7b
		//RTLMIN-5479"fsin.ps f27, f0\n"                                   // opcode=0x58600dfb
		"fnmsub.ps f16, f13, f1, f13, dyn\n"                  // opcode=0x6c16f85b
		"flw.ps f1,  32 (x4)\n"
		"fmvz.x.ps x18, f12, 4\n"                             // opcode=0xe046097b
		"fround.ps f1, f14, dyn\n"                            // opcode=0x581770fb
		"flw.ps f1,  32 (x4)\n"
		"maskor m4, m2, m2\n"                                 // opcode=0x6621627b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmax.ps f24, f12, f13\n"                             // opcode=0x28d61c7b
		"fnmadd.s f31, f13, f13, f12, dyn\n"                  // opcode=0x60d6ffcf


		"fsgnj.ps f22, f12, f13\n"                            // opcode=0x20d60b7b
		"fmin.pi f31, f14, f15\n"                             // opcode=0x2ef70ffb
		"fmsub.s f29, f15, f1, f13, dyn\n"                    // opcode=0x6817fec7
		"flw.ps f1,  32 (x4)\n"
		"fcvt.sn8.ps f21, f14\n"                              // opcode=0xd9b70afb
		"fsgnjn.ps f29, f12, f1\n"                            // opcode=0x20161efb
		"fexp.ps f16, f0\n"                                   // opcode=0x5840087b
		"fsgnjx.ps f24, f1, f1\n"                             // opcode=0x2010ac7b
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f28, f12, f15\n"                            // opcode=0x2ef63e7b
		"fltm.ps m5, f0, f13\n"                               // opcode=0xa0d052fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f16, f12, rup\n"                         // opcode=0xd016387b
		"feq.ps f24, f14, f1\n"                               // opcode=0xa0172c7b
		"fltu.pi f31, f13, f13\n"                             // opcode=0xa6d6bffb
		"cubefaceidx.ps f30, f1, f13\n"                       // opcode=0x88d09f7b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f20, x14, rne\n"                           // opcode=0xd0170a53
		"fmv.x.s x1, f0\n"                                    // opcode=0xe00000d3
		"fcvt.sn16.ps f30, f14\n"                             // opcode=0xd9970f7b
		"fround.ps f1, f0, dyn\n"                             // opcode=0x581070fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f20, x15\n"                                  // opcode=0xf0078a53
		"fmsub.ps f30, f15, f14, f14, dyn\n"                  // opcode=0x72e7ff5b
		"fsgnj.s f22, f15, f0\n"                              // opcode=0x20078b53
		"fcvt.f16.ps f29, f14\n"                              // opcode=0xd8970efb
		"fmvz.x.ps x24, f15, 5\n"                             // opcode=0xe0578c7b
		"fsgnjn.s f19, f12, f14\n"                            // opcode=0x20e619d3
		"fpackrepb.pi f22, f12\n"                             // opcode=0x26060b7b
		"fsgnjx.s f0, f13, f0\n"                              // opcode=0x2006a053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pw.ps f30, f14, rne\n"                          // opcode=0xc0070f7b
		"mov.m.x m1, x13, 66\n"                               // opcode=0x5706c0fb
		"fsetm.pi m3, f12\n"                                  // opcode=0xe00651fb
		"maskpopc x25, m0\n"                                  // opcode=0x52000cfb
		"fsgnjx.ps f24, f0, f13\n"                            // opcode=0x20d02c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw.ps f30, 1813(x14)\n"                             // opcode=0x7157af0b
		"fmax.ps f17, f12, f0\n"                              // opcode=0x280618fb
		"mova.x.m x0\n"                                       // opcode=0xd600007b
		"fcvt.sn16.ps f0, f1\n"                               // opcode=0xd990807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f21, f1, f14\n"                             // opcode=0x20e09ad3
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f29, f14, f15\n"                           // opcode=0x20f71efb
		"cubefaceidx.ps f27, f15, f1\n"                       // opcode=0x88179dfb
		"fmv.s.x f20, x14\n"                                  // opcode=0xf0070a53
		"fsub.ps f16, f15, f14, dyn\n"                        // opcode=0x08e7f87b


		"flw.ps f17, 514(x14)\n"                              // opcode=0x2026288b
		"cubesgntc.ps f22, f13, f1\n"                         // opcode=0x8816bb7b
		"fclass.s x29, f1\n"                                  // opcode=0xe0009ed3
		"fsrl.pi f0, f15, f1\n"                               // opcode=0x0617d07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.s x23, f13, f12\n"                               // opcode=0xa0c69bd3
		"fnot.pi f26, f1\n"                                   // opcode=0x0600ad7b
		"fcvt.w.s x18, f12, dyn\n"                            // opcode=0xc0067953
		"fmsub.s f28, f15, f12, f0, dyn\n"                    // opcode=0x00c7fe47
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x22\n"                                      // opcode=0xd6000b7b

		"li x5, 0x2c722ba11e2ed1b3\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsrl.pi f25, f1, f14\n"                              // opcode=0x06e0dcfb
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f22, f0, f0\n"                                // opcode=0xa6000b7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fslli.pi f27, f12, 7\n"                              // opcode=0x4ef61dfb
		"fmin.pi f1, f14, f0\n"                               // opcode=0x2e0700fb
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f19, f1, 72\n"                              // opcode=0x250089bf
		"flw.ps f1,  32 (x4)\n"
		"maskxor m5, m3, m3\n"                                // opcode=0x6631c2fb
		"fbcx.ps f16, x15\n"                                  // opcode=0x0007b80b
		"fbci.ps f24, 491602\n"                               // opcode=0xf00a5c1f
		"fmsub.ps f25, f0, f12, f13, dyn\n"                   // opcode=0x6ac07cdb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f24, f12\n"                                  // opcode=0x58260c7b


		"fsw f12, -997(x12)\n"                                // opcode=0xc0c62da7
		"fsat8.pi f22, f15\n"                                 // opcode=0x0607bb7b
		"flog.ps f28, f14\n"                                  // opcode=0x58370e7b
		"fmsub.ps f26, f13, f0, f0, rup\n"                    // opcode=0x0206bd5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvs.x.ps x29, f15, 0\n"                             // opcode=0xe007aefb
		"fcmov.ps f21, f1, f0, f1\n"                          // opcode=0x0c00aabf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmax.pi f17, f0, f14\n"                              // opcode=0x2ee018fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fexp.ps f26, f14\n"                                  // opcode=0x58470d7b
		"fsub.ps f25, f14, f14, dyn\n"                        // opcode=0x08e77cfb
		"fsub.pi f20, f15, f12\n"                             // opcode=0x0ec78a7b
		"fsub.pi f1, f15, f13\n"                              // opcode=0x0ed780fb
		"flw.ps f1,  32 (x4)\n"
		"fmax.ps f18, f12, f0\n"                              // opcode=0x2806197b
		"flt.s x16, f1, f1\n"                                 // opcode=0xa0109853
		"flw.ps f1,  32 (x4)\n"


		"flq2 f24, 2016(x15)\n"                               // opcode=0x56375c07
		"flog.ps f27, f15\n"                                  // opcode=0x58378dfb
		"fcvt.un16.ps f16, f0\n"                              // opcode=0xd910087b
		"fcvt.f16.ps f23, f13\n"                              // opcode=0xd8968bfb
		"feq.s x28, f0, f12\n"                                // opcode=0xa0c02e53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x19, m4\n"                                  // opcode=0x520209fb
		"cubefaceidx.ps f29, f12, f15\n"                      // opcode=0x88f61efb
		"flem.ps m6, f0, f0\n"                                // opcode=0xa000437b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un2.ps f30, f13\n"                              // opcode=0xd9768f7b
		"flt.pi f31, f13, f1\n"                               // opcode=0xa6169ffb
		"cubesgnsc.ps f16, f15, f0\n"                         // opcode=0x8807a87b
		"fmax.ps f30, f13, f0\n"                              // opcode=0x28069f7b
		"fandi.pi f24, f13, 364\n"                            // opcode=0xb5969c3f




		"fpackreph.pi f25, f14\n"                             // opcode=0x26071cfb


		"fbc.ps f28, 960(x13)\n"                              // opcode=0x17070e0b
		"flt.pi f28, f1, f12\n"                               // opcode=0xa6c09e7b
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x25, m4\n"                                 // opcode=0x54020cfb
		"fmvs.x.ps x21, f13, 1\n"                             // opcode=0xe016aafb
		"fclass.s x31, f13\n"                                 // opcode=0xe0069fd3
		"fminu.pi f19, f1, f14\n"                             // opcode=0x2ee0a9fb
		"flw.ps f1,  32 (x4)\n"
		"for.pi f21, f0, f12\n"                               // opcode=0x06c06afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsat8.pi f23, f1\n"                                  // opcode=0x0600bbfb
		"frcp.ps f28, f12\n"                                  // opcode=0x58760e7b
		"flt.ps f17, f14, f14\n"                              // opcode=0xa0e718fb
		"fmulh.pi f23, f14, f1\n"                             // opcode=0x16172bfb
		"fsgnjn.s f18, f1, f0\n"                              // opcode=0x20009953
		"flw.ps f1,  32 (x4)\n"


		"fmsub.ps f24, f14, f14, f12, dyn\n"                  // opcode=0x62e77c5b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsub.s f20, f0, f15, dyn\n"                          // opcode=0x08f07a53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f26, f12\n"                             // opcode=0x26061d7b
		"fltm.ps m4, f13, f14\n"                              // opcode=0xa0e6d27b
		"fmul.pi f27, f13, f15\n"                             // opcode=0x16f68dfb


		"flw.ps f1, 896(x12)\n"                               // opcode=0x3807a08b
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f31, x1\n"                                   // opcode=0xf0008fd3
		"fcvt.wu.s x23, f15, dyn\n"                           // opcode=0xc017fbd3
		"maskand m0, m1, m2\n"                                // opcode=0x6620f07b
		"fcvt.sn16.ps f0, f13\n"                              // opcode=0xd996807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flq2 f26, 1600(x13)\n"                               // opcode=0x5d965d07
		"flem.ps m3, f13, f13\n"                              // opcode=0xa0d6c1fb




		"faddi.pi f25, f13, 70\n"                             // opcode=0x24d68cbf
		"fmulhu.pi f24, f14, f12\n"                           // opcode=0x16c71c7b
		"fsetm.pi m4, f14\n"                                  // opcode=0xe007527b
		//RTLMIN-5479"frsq.ps f27, f15\n"                                  // opcode=0x58878dfb
		"fmsub.ps f30, f12, f15, f1, dyn\n"                   // opcode=0x0af67f5b
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f16, f12, f15\n"                      // opcode=0x88f6187b
		"fsatu8.pi f29, f1\n"                                 // opcode=0x0610befb
		"maskor m6, m1, m5\n"                                 // opcode=0x6650e37b




		"flem.ps m5, f14, f15\n"                              // opcode=0xa0f742fb
		"flt.pi f21, f1, f13\n"                               // opcode=0xa6d09afb
		"flw.ps f1,  32 (x4)\n"




		"fbcx.ps f29, x14\n"                                  // opcode=0x00073e8b
		"fandi.pi f24, f15, 406\n"                            // opcode=0xccd79c3f
		"mova.x.m x24\n"                                      // opcode=0xd6000c7b
		"maskpopc x1, m7\n"                                   // opcode=0x520380fb
		"fsgnjn.s f30, f12, f12\n"                            // opcode=0x20c61f53
		"fcvt.ps.f16 f1, f1\n"                                // opcode=0xd0a080fb
		"flw.ps f1,  32 (x4)\n"


		"flq2 f29, 64(x11)\n"                                 // opcode=0x2d475e87
		"fmsub.s f18, f13, f12, f15, dyn\n"                   // opcode=0x78c6f947
		"fmax.ps f1, f13, f0\n"                               // opcode=0x280690fb
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f16, f15, f15\n"                             // opcode=0xa6f7b87b
		"maskor m1, m7, m1\n"                                 // opcode=0x6613e0fb
		"feq.pi f1, f0, f1\n"                                 // opcode=0xa61020fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fpackreph.pi f29, f12\n"                             // opcode=0x26061efb
		"fand.pi f21, f13, f0\n"                              // opcode=0x0606fafb
		"for.pi f27, f12, f15\n"                              // opcode=0x06f66dfb
		"mova.x.m x20\n"                                      // opcode=0xd6000a7b
		"maskpopc x19, m3\n"                                  // opcode=0x520189fb
		"fnmadd.ps f31, f15, f14, f1, dyn\n"                  // opcode=0x0ee7ffdb
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f20, f12, f12\n"                              // opcode=0x28c60a53
		"fcmov.ps f27, f13, f13, f12\n"                       // opcode=0x64d6adbf
		"fltu.pi f22, f1, f0\n"                               // opcode=0xa600bb7b
		"flw.ps f1,  32 (x4)\n"
		"fmulhu.pi f16, f13, f14\n"                           // opcode=0x16e6987b
		"fle.pi f21, f15, f12\n"                              // opcode=0xa6c78afb
		"fmv.x.s x17, f14\n"                                  // opcode=0xe00708d3
		"fmax.s f23, f0, f13\n"                               // opcode=0x28d01bd3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f25, f14, f1\n"                               // opcode=0x06176cfb
		"fpackreph.pi f28, f12\n"                             // opcode=0x26061e7b
		"ffrc.ps f27, f15\n"                                  // opcode=0x58278dfb
		"fcvt.pw.ps f23, f15, dyn\n"                          // opcode=0xc007fbfb

		"li x5, 0xb123273ebddca02d\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.f16.ps f1, f1\n"                                // opcode=0xd89080fb
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f24, f0, f15, f0, dyn\n"                     // opcode=0x00f07c43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f27, f14\n"                             // opcode=0x26071dfb
		"fround.ps f26, f13, dyn\n"                           // opcode=0x5816fd7b
		"fcvt.un8.ps f21, f15\n"                              // opcode=0xd9378afb
		"maskor m2, m6, m7\n"                                 // opcode=0x6673617b
		"fmax.ps f28, f15, f1\n"                              // opcode=0x28179e7b
		"fcmov.ps f31, f14, f13, f12\n"                       // opcode=0x64d72fbf
		"maskpopc x29, m3\n"                                  // opcode=0x52018efb
		"fle.s x17, f15, f15\n"                               // opcode=0xa0f788d3
		"fcvt.wu.s x30, f1, dyn\n"                            // opcode=0xc010ff53
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f28, f15, f13\n"                              // opcode=0xa6d79e7b
		"fclass.s x16, f12\n"                                 // opcode=0xe0061853
		"flt.pi f23, f14, f12\n"                              // opcode=0xa6c71bfb
		"fclass.s x29, f14\n"                                 // opcode=0xe0071ed3
		"fnot.pi f22, f14\n"                                  // opcode=0x06072b7b
		"mova.m.x x13\n"                                      // opcode=0xd606907b
		"fmsub.s f29, f0, f13, f12, dyn\n"                    // opcode=0x60d07ec7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.ps f1, f0, f1\n"                                 // opcode=0xa01010fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsub.pi f16, f15, f14\n"                             // opcode=0x0ee7887b
		"feq.s x0, f13, f0\n"                                 // opcode=0xa006a053
		"masknot m3, m3\n"                                    // opcode=0x6601a1fb
		"fsra.pi f29, f13, f12\n"                             // opcode=0x0ec6defb
		"fmsub.s f27, f13, f12, f1, dyn\n"                    // opcode=0x08c6fdc7
		"flw.ps f1,  32 (x4)\n"
		"fminu.pi f0, f12, f15\n"                             // opcode=0x2ef6207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f18, f0\n"                              // opcode=0x2600197b
		"fsub.s f1, f15, f1, dyn\n"                           // opcode=0x0817f0d3
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m1, x1, 15\n"                                // opcode=0x5630f0fb
		"fcvt.ps.pw f19, f14, dyn\n"                          // opcode=0xd00779fb
		"fsll.pi f1, f12, f14\n"                              // opcode=0x06e610fb
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f29, 20922\n"                                // opcode=0x0a374e9f
		"fmv.s.x f16, x11\n"                                  // opcode=0xf0058853
		"fsrai.pi f18, f15, 7\n"                              // opcode=0x4ee7f97b
		"flem.ps m0, f1, f12\n"                               // opcode=0xa0c0c07b
		"flw.ps f1,  32 (x4)\n"
		"fsrai.pi f23, f0, 5\n"                               // opcode=0x4ea07bfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fandi.pi f19, f14, 391\n"                            // opcode=0xc4e719bf
		"flt.s x18, f14, f12\n"                               // opcode=0xa0c71953
		"fsll.pi f20, f15, f14\n"                             // opcode=0x06e79a7b
		"mova.x.m x21\n"                                      // opcode=0xd6000afb
		"fcvt.pwu.ps f31, f14, dyn\n"                         // opcode=0xc0177ffb
		"fmin.ps f19, f12, f13\n"                             // opcode=0x28d609fb
		"fcvt.ps.pw f1, f13, dyn\n"                           // opcode=0xd006f0fb
		"flw.ps f1,  32 (x4)\n"




		"maskpopc x29, m5\n"                                  // opcode=0x52028efb
		"fmax.ps f25, f13, f12\n"                             // opcode=0x28c69cfb






		"for.pi f17, f1, f12\n"                               // opcode=0x06c0e8fb
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f20, x0\n"                                   // opcode=0xf0000a53
		"fxor.pi f21, f12, f15\n"                             // opcode=0x06f64afb
		"fcvt.w.s x27, f15, dyn\n"                            // opcode=0xc007fdd3
		"fsub.ps f27, f1, f13, dyn\n"                         // opcode=0x08d0fdfb
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.ps f18, f13, f13, f0, dyn\n"                  // opcode=0x06d6f95b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.f16.ps f16, f15\n"                              // opcode=0xd897887b
		"frcp.ps f19, f15\n"                                  // opcode=0x587789fb
		"fle.ps f26, f12, f15\n"                              // opcode=0xa0f60d7b


		"fsw f1, -1219(x11)\n"                                // opcode=0xb210aea7
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pw f30, f12, dyn\n"                          // opcode=0xd0067f7b
		"fmv.x.s x24, f0\n"                                   // opcode=0xe0000c53
		"fcvt.un2.ps f30, f15\n"                              // opcode=0xd9778f7b
		"fclass.ps f21, f13\n"                                // opcode=0xe0069afb
		"frcp.ps f1, f14\n"                                   // opcode=0x587700fb
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f1, f13, f0\n"                               // opcode=0x0606c0fb
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f25, f13, f12\n"                           // opcode=0x00c68cf7


		"flw f25, 1724(x11)\n"                                // opcode=0x6bc62c87
		"flt.pi f28, f15, f15\n"                              // opcode=0xa6f79e7b
		"frcp.ps f21, f13\n"                                  // opcode=0x58768afb
		"fsrli.pi f20, f14, 11\n"                             // opcode=0x4f675a7b
		"fmv.s.x f0, x13\n"                                   // opcode=0xf0068053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f25, f15\n"                                  // opcode=0x58778cfb
		"fcvt.un2.ps f0, f0\n"                                // opcode=0xd970007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f18, f13, f13\n"                             // opcode=0xa6d6b97b




		"fbci.ps f22, 44477\n"                                // opcode=0x15b7bb1f
		"fsrai.pi f27, f0, 8\n"                               // opcode=0x4f107dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f0, f15, f15\n"                          // opcode=0x88f7807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"masknot m0, m3\n"                                    // opcode=0x6601a07b
		"fcvt.un16.ps f1, f15\n"                              // opcode=0xd91780fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f29, f0, dyn\n"                          // opcode=0xd0107efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"masknot m0, m1\n"                                    // opcode=0x6600a07b


		"fsw.ps f12, -1662(x13)\n"                            // opcode=0x98c6e10b
		"fcvt.s.wu f27, x13, dyn\n"                           // opcode=0xd016fdd3
		"fcvt.un16.ps f31, f12\n"                             // opcode=0xd9160ffb


		"flw f1, -1902(x12)\n"                                // opcode=0x8920a087
		"flw.ps f1,  32 (x4)\n"


		"fsgnjn.s f27, f14, f15\n"                            // opcode=0x20f71dd3
		"fnmsub.s f29, f1, f14, f0, dyn\n"                    // opcode=0x00e0fecb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.f16 f22, f14\n"                              // opcode=0xd0a70b7b
		"fbcx.ps f23, x11\n"                                  // opcode=0x0005bb8b
		"fmadd.ps f18, f12, f13, f1, dyn\n"                   // opcode=0x08d6795b
		"flw.ps f1,  32 (x4)\n"
		"fpackrepb.pi f23, f13\n"                             // opcode=0x26068bfb
		"fbci.pi f27, 479069\n"                               // opcode=0xe9ebaddf
		"fsgnjn.s f28, f0, f14\n"                             // opcode=0x20e01e53
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fslli.pi f30, f1, 14\n"                              // opcode=0x4fc09f7b
		"flw.ps f1,  32 (x4)\n"




		"fsrai.pi f21, f15, 5\n"                              // opcode=0x4ea7fafb
		"fcmovm.ps f20, f1, f1\n"                             // opcode=0x00108a77
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f31, f15\n"                                 // opcode=0x0607bffb




		"fnot.pi f19, f0\n"                                   // opcode=0x060029fb
		"fmsub.ps f29, f15, f0, f14, dyn\n"                   // opcode=0x7207fedb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f22, f14, f14\n"                             // opcode=0x28e70b7b


		"fsw.ps f14, 536(x14)\n"                              // opcode=0x20e76c0b
		"fsgnjn.s f26, f15, f12\n"                            // opcode=0x20c79d53
		"fcvt.w.s x0, f15, dyn\n"                             // opcode=0xc007f053
		"fle.pi f25, f15, f13\n"                              // opcode=0xa6d78cfb
		"fcvt.un16.ps f31, f14\n"                             // opcode=0xd9170ffb

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbci.ps f30, 514913\n"                               // opcode=0xfb6c3f1f
		"fcvt.sn8.ps f24, f0\n"                               // opcode=0xd9b00c7b
		"fnmadd.s f19, f13, f0, f15, dyn\n"                   // opcode=0x7806f9cf
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m4, x0, 15\n"                                // opcode=0x5630627b
		"fsgnj.s f31, f15, f13\n"                             // opcode=0x20d78fd3
		"fpackreph.pi f26, f0\n"                              // opcode=0x26001d7b


		"fmaxu.pi f29, f15, f15\n"                            // opcode=0x2ef7befb
		"flt.pi f27, f13, f12\n"                              // opcode=0xa6c69dfb
		"fmulh.pi f21, f14, f1\n"                             // opcode=0x16172afb


		"fbc.ps f1, -416(x15)\n"                              // opcode=0xaa67808b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un16.ps f26, f12\n"                             // opcode=0xd9160d7b
		"fmaxu.pi f0, f0, f0\n"                               // opcode=0x2e00307b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.ps m3, f0, f15\n"                               // opcode=0xa0f051fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f22, 126201\n"                               // opcode=0x3d9f2b5f
		"fle.ps f1, f1, f0\n"                                 // opcode=0xa00080fb
		"flw.ps f1,  32 (x4)\n"
		"fmvz.x.ps x21, f0, 1\n"                              // opcode=0xe0100afb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fround.ps f22, f12, dyn\n"                           // opcode=0x58167b7b
		"fmulh.pi f1, f13, f14\n"                             // opcode=0x16e6a0fb
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f18, f13, f12\n"                            // opcode=0x2ec6b97b
		"fcmovm.ps f23, f15, f14\n"                           // opcode=0x00e78bf7
		"fcvt.ps.pwu f20, f1, dyn\n"                          // opcode=0xd010fa7b
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un8.ps f21, f12\n"                              // opcode=0xd9360afb


		"fsw f15, -438(x15)\n"                                // opcode=0xe4f0a527
		"fsgnjx.s f22, f13, f13\n"                            // opcode=0x20d6ab53
		"fcvt.s.wu f20, x15, dyn\n"                           // opcode=0xd017fa53
		"fsub.ps f17, f12, f1, dyn\n"                         // opcode=0x081678fb
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x20, f14\n"                                  // opcode=0xe0070a53
		"fsatu8.pi f16, f0\n"                                 // opcode=0x0610387b
		"flem.ps m5, f14, f13\n"                              // opcode=0xa0d742fb
		"fltm.pi m1, f12, f1\n"                               // opcode=0x3e1600fb
		"fmvz.x.ps x28, f0, 0\n"                              // opcode=0xe0000e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fmulhu.pi f18, f15, f1\n"                            // opcode=0x1617997b




		"fround.ps f30, f0, dyn\n"                            // opcode=0x58107f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x24, f0, 3\n"                              // opcode=0xe0300c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgnsc.ps f22, f14, f14\n"                        // opcode=0x88e72b7b
		"fsat8.pi f17, f0\n"                                  // opcode=0x060038fb
		"flt.s x29, f0, f14\n"                                // opcode=0xa0e01ed3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.un16.ps f0, f0\n"                               // opcode=0xd910007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw f0, -1091(x15)\n"                                // opcode=0xba07aea7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m5, m0, m3\n"                                 // opcode=0x663062fb
		"fcvt.f16.ps f1, f13\n"                               // opcode=0xd89680fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f30, f14, dyn\n"                          // opcode=0xc0077f7b
		"fcmov.ps f30, f14, f14, f15\n"                       // opcode=0x7ce72f3f
		"fmulh.pi f28, f0, f14\n"                             // opcode=0x16e02e7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f20, f12, 112\n"                           // opcode=0xe7c61a7b
		"fsat8.pi f25, f12\n"                                 // opcode=0x06063cfb
		"fpackreph.pi f27, f0\n"                              // opcode=0x26001dfb
		"fbci.pi f26, 239896\n"                               // opcode=0x75231d5f
		"fcvt.un10.ps f16, f15\n"                             // opcode=0xd927887b
		"fltu.pi f18, f1, f0\n"                               // opcode=0xa600b97b
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmsub.ps f18, f0, f0, f14, dyn\n"                    // opcode=0x7200795b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f30, f1, dyn\n"                           // opcode=0xd000ff7b
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f18, f14, f1\n"                            // opcode=0x2017197b
		"fadd.ps f30, f13, f0, dyn\n"                         // opcode=0x0006ff7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f23, f15, f0\n"                       // opcode=0x88079bfb
		"fcvt.ps.f16 f20, f15\n"                              // opcode=0xd0a78a7b




		"fbci.ps f1, 482347\n"                                // opcode=0xeb85709f
		"flw.ps f1,  32 (x4)\n"
		"fmvs.x.ps x23, f13, 0\n"                             // opcode=0xe006abfb
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"feq.pi f27, f12, f12\n"                              // opcode=0xa6c62dfb
		"fand.pi f31, f14, f1\n"                              // opcode=0x06177ffb
		"fclass.ps f23, f12\n"                                // opcode=0xe0061bfb
		"fandi.pi f29, f15, 352\n"                            // opcode=0xb4179ebf
		"fsgnjn.ps f25, f0, f15\n"                            // opcode=0x20f01cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.pi f17, f1, f12\n"                               // opcode=0xa6c0a8fb
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x19, m6\n"                                 // opcode=0x540309fb
		"fnot.pi f18, f0\n"                                   // opcode=0x0600297b
		"fnmadd.ps f16, f0, f12, f1, dyn\n"                   // opcode=0x0ec0785b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskor m5, m5, m0\n"                                 // opcode=0x6602e2fb
		"fcvt.un8.ps f21, f12\n"                              // opcode=0xd9360afb
		"fcvt.un2.ps f25, f1\n"                               // opcode=0xd9708cfb
		"fpackreph.pi f26, f13\n"                             // opcode=0x26069d7b
		"fsatu8.pi f16, f0\n"                                 // opcode=0x0610387b
		"fsat8.pi f20, f12\n"                                 // opcode=0x06063a7b
		"fbci.ps f21, 136304\n"                               // opcode=0x428e0a9f
		"fmax.pi f29, f1, f0\n"                               // opcode=0x2e009efb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.un10.ps f31, f13\n"                             // opcode=0xd9268ffb
		//RTLMIN-5479"fsin.ps f27, f12\n"                                  // opcode=0x58660dfb
		//RTLMIN-5479"fsin.ps f0, f1\n"                                    // opcode=0x5860807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.ps f20, f1, f0, dyn\n"                          // opcode=0x0000fa7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fltm.ps m0, f0, f12\n"                               // opcode=0xa0c0507b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f29, f13, 54\n"                             // opcode=0x1cc68ebf


		"flw f26, 2040(x14)\n"                                // opcode=0x7f85ad07
		"fltm.pi m4, f14, f12\n"                              // opcode=0x3ec7027b
		"masknot m3, m2\n"                                    // opcode=0x660121fb
		"cubesgntc.ps f18, f13, f14\n"                        // opcode=0x88e6b97b
		"maskpopc x23, m4\n"                                  // opcode=0x52020bfb
		"fbci.ps f21, 24283\n"                                // opcode=0x0bdb7a9f
		"fcvt.ps.f16 f31, f13\n"                              // opcode=0xd0a68ffb
		"fcvt.w.s x19, f1, dyn\n"                             // opcode=0xc000f9d3
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f23, f12, 309\n"                            // opcode=0x9cb60bbf






		"flw f1, 964(x12)\n"                                  // opcode=0x3c402087
		"flw.ps f1,  32 (x4)\n"


		"flw f26, -1351(x13)\n"                               // opcode=0xab96ad07
		"fnmsub.s f28, f0, f13, f13, dyn\n"                   // opcode=0x68d07e4b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f0, f0, 5\n"                                // opcode=0x4ea0707b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m3, x1, 111\n"                               // opcode=0x57b0e1fb
		"fmsub.s f27, f0, f14, f12, dyn\n"                    // opcode=0x60e07dc7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.s x29, f12, f14\n"                               // opcode=0xa0e62ed3
		"fminu.pi f30, f14, f13\n"                            // opcode=0x2ed72f7b
		"fxor.pi f24, f1, f13\n"                              // opcode=0x06d0cc7b
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0x9b22988bcc0a1cd9\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.f16.ps f17, f15\n"                              // opcode=0xd89788fb
		"fsrli.pi f21, f14, 6\n"                              // opcode=0x4ec75afb
		"fcvt.f16.ps f22, f13\n"                              // opcode=0xd8968b7b


		"flw f19, 191(x15)\n"                                 // opcode=0x0bf02987
		"fcmov.ps f20, f13, f1, f12\n"                        // opcode=0x6416aa3f
		"flw.ps f1,  32 (x4)\n"
		"feq.s x21, f13, f0\n"                                // opcode=0xa006aad3
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
