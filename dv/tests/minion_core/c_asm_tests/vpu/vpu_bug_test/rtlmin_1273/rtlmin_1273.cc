/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

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
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0x7f800000,    // inf                   
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           

      // vpu register f3
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0xcb000000,    // -8388608.0            
      0x7fc00001,    // signaling NaN         
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 

      // vpu register f4
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  

      // vpu register f5
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f6
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  

      // vpu register f7
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f8
      0x4b000000,    // 8388608.0             
      0x00000000,    // zero                  
      0x7f800000,    // inf                   
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         

      // vpu register f9
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f10
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  

      // vpu register f11
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f12
      0x7f800000,    // inf                   
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0xffc00001,    // -signaling NaN        
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f13
      0xcb000000,    // -8388608.0            
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  

      // vpu register f14
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  

      // vpu register f15
      0xffc00001,    // -signaling NaN        
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x00000000,    // zero                  

      // vpu register f16
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  

      // vpu register f17
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 

      // vpu register f18
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  

      // vpu register f19
      0x00011111,    // 9.7958E-41            
      0x00000000,    // zero                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           

      // vpu register f20
      0x7fc00001,    // signaling NaN         
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        

      // vpu register f21
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f22
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f23
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   

      // vpu register f24
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            

      // vpu register f25
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 

      // f26
      0xffffffff,    
      0xffffffff,   
      0xffffffff,  
      0xffffffff, 
      0xffffffff,  
      0xffffffff, 
      0xffffffff,  
      0xffffffff, 

      // f27
      0xffffffff,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,

      // f28
      0x82920000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,

      // f29
      0x82920000,
      0x7fc00000,
      0x00000000,
      0x4b000000,
      0xcb8c4b40,
      0x4b000000,
      0x7fc00000,
      0xbf800000,

      // f30
      0xffffffff,    
      0xffffffff,   
      0xffffffff,  
      0xffffffff, 
      0xffffffff,  
      0xffffffff, 
      0xffffffff,  
      0xffffffff, 

      // f31
      0x7fc00000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000
   };

   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f26,  832 (%[operands])\n"         // 0x3407ad0b
         "flw.ps f27,  864 (%[operands])\n"         // 0x3607ad8b
         "flw.ps f28,  896 (%[operands])\n"         // 0x3807ae0b
         "flw.ps f30,  960 (%[operands])\n"         // 0x3c07af0b
         "flw.ps f31,  992 (%[operands])\n"         // 0x3e07af8b
         "mov.m.x m0, zero, 0x00\n"  // 0x5600007b                     
         "fadd.s f26, f27, f28\n"    // 0x01cdfd53    
         "fadd.ps f26, f27, f28\n"   // 0x01cdfd7b 
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f26", "f27", "f28", "f30", "f31"
         );
   C_TEST_PASS;
   return 0;
}
