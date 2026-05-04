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
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000000,                                                  // -zero                                      
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80000000,                                                  // -zero                                      
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x55555555,                                                  // 4 random values                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000000,                                                  // zero                                       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000000,                                                  // zero                                       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c780000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0c700000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff800000,                                                  // -inf                                       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800000,                                                  // inf                                        
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f800000,                                                  // inf                                        
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x55555555,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x55555555,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000000,                                                  // zero                                       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f800000,                                                  // inf                                        
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x33333333,                                                  // 4 random values                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000000,                                                  // zero                                       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x55555555,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x55555555,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x55555555,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000000,                                                  // zero                                       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x55555555,                                                  // 4 random values                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000000,                                                  // zero                                       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff800000,                                                  // -inf                                       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000000,                                                  // zero                                       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x33333333,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f800000,                                                  // inf                                        
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff800000,                                                  // -inf                                       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x33333333,                                                  // 4 random values                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x55555555,                                                  // 4 random values                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff800000,                                                  // -inf                                       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x7f800000,                                                  // inf                                        
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c780000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff800000,                                                  // -inf                                       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
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
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  

			 // vpu register f3
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                

			 // vpu register f4
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f5
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f6
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               

			 // vpu register f7
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 

			 // vpu register f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                

			 // vpu register f9
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f10
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      

			 // vpu register f11
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f12
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             

			 // vpu register f13
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               

			 // vpu register f14
			 0x4b000000,                                                  // 8388608.0                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                

			 // vpu register f15
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f16
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001,                                                  // -signaling NaN                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                

			 // vpu register f17
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f18
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  

			 // vpu register f19
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f20
			 0xff800000,                                                  // -inf                                       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            

			 // vpu register f21
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                

			 // vpu register f22
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            

			 // vpu register f23
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               

			 // vpu register f24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                

			 // vpu register f25
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            

			 // vpu register f26
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            

			 // vpu register f27
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f28
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f29
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               

			 // vpu register f30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f31
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
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
		"fcvt.ps.pwu f30, f12, dyn\n"                         // opcode=0xd0167f7b
		"fsgnj.ps f29, f1, f0\n"                              // opcode=0x20008efb
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f29, f0, f15\n"                             // opcode=0x20f00efb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"fsin.ps f0, f14\n"                                   // opcode=0x5867007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f25, f0, f13\n"                               // opcode=0x06d06cfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fsrl.pi f24, f15, f14\n"                             // opcode=0x06e7dc7b




		"fandi.pi f28, f14, 506\n"                            // opcode=0xfd471e3f




		"fcvt.pw.ps f20, f15, dyn\n"                          // opcode=0xc007fa7b
		"fsrl.pi f29, f15, f13\n"                             // opcode=0x06d7defb
		"fmin.pi f18, f13, f1\n"                              // opcode=0x2e16897b
		"faddi.pi f31, f1, 115\n"                             // opcode=0x3c608fbf
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f30, f1, f0, f15, dyn\n"                     // opcode=0x7800ff43
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsrli.pi f24, f14, 1\n"                              // opcode=0x4e275c7b
		"fmax.s f0, f1, f15\n"                                // opcode=0x28f09053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmin.pi f27, f12, f15\n"                             // opcode=0x2ef60dfb
		"fbci.ps f0, 72159\n"                                 // opcode=0x233bf01f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.s.x f17, x1\n"                                   // opcode=0xf00088d3
		"masknot m0, m1\n"                                    // opcode=0x6600a07b
		"fsgnjx.s f28, f12, f0\n"                             // opcode=0x20062e53
		"fmaxu.pi f0, f12, f14\n"                             // opcode=0x2ee6307b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f18, f13, f12, f12, dyn\n"                  // opcode=0x60c6f94f


		"flw f23, -2036(x14)\n"                               // opcode=0x80c72b87
		"fsll.pi f0, f14, f15\n"                              // opcode=0x06f7107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f21, f15\n"                                // opcode=0x0617bafb
		"fsub.pi f0, f1, f13\n"                               // opcode=0x0ed0807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f18, f15, f13\n"                              // opcode=0xa0d7997b
		"fltm.pi m4, f14, f12\n"                              // opcode=0x3ec7027b
		"fcvt.wu.s x21, f0, dyn\n"                            // opcode=0xc0107ad3
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgntc.ps f23, f12, f14\n"                        // opcode=0x88e63bfb
		"faddi.pi f22, f1, 307\n"                             // opcode=0x9c708b3f
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f31, f14, f0, f15, dyn\n"                    // opcode=0x78077fc7
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f21, f14, f15, dyn\n"                         // opcode=0x10f77ad3
		"fle.pi f0, f13, f1\n"                                // opcode=0xa616807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f26, f14, f12\n"                             // opcode=0x0ec70d7b
		"fsrli.pi f22, f15, 9\n"                              // opcode=0x4f27db7b


		"flw.ps f0, 1218(x14)\n"                              // opcode=0x4c20200b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.s f27, f0, f1, f12, dyn\n"                    // opcode=0x60107dcb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f18, f13, f15\n"                             // opcode=0x16f6897b
		"fcvt.s.w f26, x15, dyn\n"                            // opcode=0xd007fd53
		"fcvt.f16.ps f1, f14\n"                               // opcode=0xd89700fb
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f29, x15\n"                                  // opcode=0xf0078ed3
		"fnmadd.ps f28, f1, f1, f14, dyn\n"                   // opcode=0x7610fe5b
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f20, f0, 23\n"                             // opcode=0xe6507a7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m6, f13\n"                                  // opcode=0xe006d37b


		"flq2 f19, -1920(x13)\n"                              // opcode=0xb8b6d987
		"fsgnjx.ps f30, f13, f0\n"                            // opcode=0x2006af7b
		"fsatu8.pi f19, f0\n"                                 // opcode=0x061039fb
		"ffrc.ps f29, f12\n"                                  // opcode=0x58260efb
		"fsgnjn.ps f24, f13, f14\n"                           // opcode=0x20e69c7b
		"fmin.pi f31, f12, f13\n"                             // opcode=0x2ed60ffb
		"fnmadd.ps f16, f13, f13, f14, dyn\n"                 // opcode=0x76d6f85b
		"fpackrepb.pi f1, f15\n"                              // opcode=0x260780fb
		"flw.ps f1,  32 (x4)\n"
		"maskor m4, m7, m1\n"                                 // opcode=0x6613e27b
		"fmul.s f24, f15, f12, dyn\n"                         // opcode=0x10c7fc53
		"fmsub.ps f16, f12, f1, f14, dyn\n"                   // opcode=0x7216785b
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f18, f0, f13\n"                               // opcode=0xa0d0297b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f24, f1, f1\n"                               // opcode=0x0e108c7b
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f0, f12, f12\n"                              // opcode=0x06c6407b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fswizz.ps f30, f15, 26\n"                            // opcode=0xe667df7b
		"fslli.pi f17, f12, 4\n"                              // opcode=0x4e9618fb
		"fand.pi f17, f1, f1\n"                               // opcode=0x0610f8fb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f16, x13, dyn\n"                            // opcode=0xd006f853
		"fmadd.ps f1, f14, f12, f12, dyn\n"                   // opcode=0x60c770db
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f21, f12, f14\n"                             // opcode=0x16e60afb
		"fle.ps f19, f13, f0\n"                               // opcode=0xa00689fb
		"frcp.ps f16, f12\n"                                  // opcode=0x5876087b
		"mova.m.x x1\n"                                       // opcode=0xd600907b
		"maskand m2, m3, m1\n"                                // opcode=0x6611f17b
		"fbci.pi f0, 361146\n"                                // opcode=0xb057505f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.f16 f1, f13\n"                               // opcode=0xd0a680fb
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f24, f0, f1\n"                               // opcode=0x06104c7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.ps f31, f0, f15, f15, dyn\n"                  // opcode=0x7cf07fdb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f31, f14, dyn\n"                         // opcode=0xc0177ffb
		"fsgnjx.s f20, f1, f13\n"                             // opcode=0x20d0aa53
		"flw.ps f1,  32 (x4)\n"






		"fsrli.pi f21, f15, 0\n"                              // opcode=0x4e07dafb
		"fsgnjn.ps f1, f0, f1\n"                              // opcode=0x201010fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.s f17, f12, f15, f1, dyn\n"                   // opcode=0x08f678cf
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x19, f14\n"                                 // opcode=0xe00719d3
		"maskpopcz x16, m0\n"                                 // opcode=0x5400087b
		"faddi.pi f21, f1, 333\n"                             // opcode=0xa5b08abf
		"flw.ps f1,  32 (x4)\n"


		"fsw f13, -1905(x11)\n"                               // opcode=0x88d0a7a7
		"cubeface.ps f26, f12, f13\n"                         // opcode=0x88d60d7b
		"flt.s x20, f1, f13\n"                                // opcode=0xa0d09a53
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f26, 148372\n"                               // opcode=0x48728d1f
		"maskor m2, m7, m5\n"                                 // opcode=0x6653e17b




		"maskxor m1, m6, m6\n"                                // opcode=0x666340fb


		"maskpopc x23, m5\n"                                  // opcode=0x52028bfb
		"fadd.pi f24, f14, f14\n"                             // opcode=0x06e70c7b
		"fltu.pi f18, f0, f1\n"                               // opcode=0xa610397b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f27, f1\n"                                   // opcode=0x58808dfb
		"fmin.s f21, f14, f0\n"                               // opcode=0x28070ad3


		"flw.ps f30, -218(x15)\n"                             // opcode=0xf2602f0b
		"fround.ps f27, f0, dyn\n"                            // opcode=0x58107dfb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f30, 515152\n"                               // opcode=0xfb8a1f1f
		"faddi.pi f20, f14, 189\n"                            // opcode=0x5da70a3f


		"flw.ps f20, 288(x13)\n"                              // opcode=0x12002a0b
		"fcmov.ps f22, f13, f12, f15\n"                       // opcode=0x7cc6ab3f








		"fcvt.pw.ps f16, f15, dyn\n"                          // opcode=0xc007f87b
		"fcvt.ps.f16 f0, f13\n"                               // opcode=0xd0a6807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f30, f13, dyn\n"                         // opcode=0xc016ff7b
		"fadd.pi f17, f0, f1\n"                               // opcode=0x061008fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.s f21, f12, f13\n"                            // opcode=0x20d62ad3
		"fsatu8.pi f31, f15\n"                                // opcode=0x0617bffb
		"fround.ps f17, f14, dyn\n"                           // opcode=0x581778fb
		//RTLMIN-5479"frsq.ps f27, f1\n"                                   // opcode=0x58808dfb
		"fsgnjx.s f22, f15, f12\n"                            // opcode=0x20c7ab53
		"fcmovm.ps f27, f15, f1\n"                            // opcode=0x00178df7
		"fltm.pi m0, f12, f15\n"                              // opcode=0x3ef6007b
		"fmv.x.s x0, f13\n"                                   // opcode=0xe0068053
		"fcvt.pw.ps f31, f14, dyn\n"                          // opcode=0xc0077ffb
		"fsgnjn.ps f19, f0, f13\n"                            // opcode=0x20d019fb
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fexp.ps f27, f14\n"                                  // opcode=0x58470dfb
		"fcvt.f16.ps f28, f14\n"                              // opcode=0xd8970e7b
		"fsra.pi f18, f14, f12\n"                             // opcode=0x0ec7597b
		"fsrl.pi f1, f1, f13\n"                               // opcode=0x06d0d0fb
		"flw.ps f1,  32 (x4)\n"
		"fnmsub.s f1, f12, f12, f14, dyn\n"                   // opcode=0x70c670cb
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f25, x1, dyn\n"                            // opcode=0xd010fcd3
		"fslli.pi f28, f14, 8\n"                              // opcode=0x4f171e7b
		"fbci.ps f30, 96614\n"                                // opcode=0x2f2cdf1f
		"fcvt.ps.pw f17, f12, dyn\n"                          // opcode=0xd00678fb
		"fmaxu.pi f30, f0, f0\n"                              // opcode=0x2e003f7b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x21\n"                                      // opcode=0xd6000afb
		"fexp.ps f31, f0\n"                                   // opcode=0x58400ffb




		"fnmadd.ps f22, f13, f12, f0, dyn\n"                  // opcode=0x06c6fb5b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.ps f16, f0, f12\n"                             // opcode=0x20c0087b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f28, f13, dyn\n"                         // opcode=0xc016fe7b
		"fbci.ps f19, 520929\n"                               // opcode=0xfe5c399f
		"masknot m6, m7\n"                                    // opcode=0x6603a37b
		"maskxor m0, m6, m0\n"                                // opcode=0x6603407b
		"fmv.x.s x30, f13\n"                                  // opcode=0xe0068f53
		"frcp.ps f29, f13\n"                                  // opcode=0x58768efb
		"fsat8.pi f23, f1\n"                                  // opcode=0x0600bbfb


		"flw.ps f21, -325(x14)\n"                             // opcode=0xebb72a8b


		"fbc.ps f25, -1792(x12)\n"                            // opcode=0xfb160c8b
		"fcvt.s.wu f20, x1, dyn\n"                            // opcode=0xd010fa53
		"fcvt.pw.ps f23, f14, dyn\n"                          // opcode=0xc0077bfb
		"fadd.ps f27, f13, f13, dyn\n"                        // opcode=0x00d6fdfb
		"faddi.pi f18, f12, 268\n"                            // opcode=0x8596093f
		"fsub.s f28, f15, f13, dyn\n"                         // opcode=0x08d7fe53
		"fsrai.pi f28, f12, 0\n"                              // opcode=0x4e067e7b
		"for.pi f16, f0, f1\n"                                // opcode=0x0610687b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f1, f15, f1\n"                                // opcode=0xa017a0fb
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f0, f13, 406\n"                             // opcode=0xccd6803f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flog.ps f27, f14\n"                                  // opcode=0x58370dfb
        
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
